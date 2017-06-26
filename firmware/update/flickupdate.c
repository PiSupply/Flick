//#include <glib.h>
//#include <glib/gprintf.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
//#include <linux/i2c.h>

#define	AIC32X4_PSEL		0

#define CRC_P_32 0xEDB88320L

#   include <assert.h>
#   define GESTIC_ASSERT(X) assert(X)

typedef struct {
    unsigned short address;
    unsigned char length;
    unsigned char data[128];
} gestic_flash_record_t;

typedef struct {
    int record_count;
    unsigned char iv[14];
    unsigned char fw_version[120];
    gestic_flash_record_t data[1];
} gestic_flash_image_t;

typedef enum {
    gestic_UpdateFunction_ProgramFlash = 0,
    gestic_UpdateFunction_VerifyOnly = 1,
    gestic_UpdateFunction_Restart = 3
} gestic_UpdateFunction_t;

gestic_UpdateFunction_t session_mode;

extern gestic_flash_image_t Loader;
extern gestic_flash_image_t Library;

#define XFER_DIR "/sys/class/gpio/gpio27/direction"
#define RESET_DIR "/sys/class/gpio/gpio17/direction"

#define XFER "/sys/class/gpio/gpio27/value"
#define RESET "/sys/class/gpio/gpio17/value"

#define INPUT "in"
#define OUTPUT "out"

#define HIGH	"1"
#define LOW		"0"

#define SET_U8(P, X) (*(uint8_t*)(P) = (unsigned char)(X))
#define SET_U32(P, X) (*(uint32_t*)(P) = (unsigned int)(X))
#define GET_U16(P) (*(uint16_t*)(P))
#define SET_U16(P, X) (*(uint16_t*)(P) = (unsigned short)(X))

unsigned int gest_session_id;

void pinMode(char* dirFile, char* mode) {
	int dir = open(dirFile, O_WRONLY);
	if(dir < 0) printf("can't open  gpio xfer direction file\n");
	write(dir, mode, strlen(mode));
	close(dir); 
}

void digitalWrite(char* pinFile, char* value) {
	int pin = open(pinFile, O_WRONLY);
	if(pin < 0) printf("can't open  pin file\n");
	write(pin, value, 1);
	close(pin); 
}

uint8_t digitalRead(char* pinFile) {
	int pin = open(pinFile, O_RDONLY);
	if(pin < 0) printf("can't open  pin file\n");
	char value;
	read(pin, &value, 1);
	close(pin); 
	return value == '1';
}

int file = -1;

static uint8_t rcvMsg[300];

uint8_t gestic_msg_System_Status = 0x15;
uint8_t gestic_msg_Request_Message = 0x06;
uint8_t gestic_msg_Fw_Update_Start = 0x80;
uint8_t gestic_msg_Fw_Update_Block = 0x81;
uint8_t gestic_msg_Fw_Update_Completed = 0x82;
uint8_t gestic_msg_Fw_Version_Info = 0x83;
uint8_t gestic_msg_Sensor_Data_Output = 0x91;
uint8_t gestic_msg_Set_Runtime_Parameter = 0xA2;

static int mgc_write(/*struct snd_soc_codec *codec,*/int file, unsigned int reg,
				unsigned int val)
{
	//struct aic32x4_priv *aic32x4 = snd_soc_codec_get_drvdata(codec);
	unsigned int page = reg / 128;
	unsigned int fixed_reg = reg % 128;
	unsigned char data[2];
	int ret;

	data[0] = fixed_reg & 0xff;
	data[1] = val & 0xff;

	if (write(file, data, 2) == 2)
		return 0;
	else {
		printf("Failed to write to aic32x4.");
		return -1;
	}

	return 0;
}

static unsigned int mgc_read(int file, unsigned int reg)
{
	unsigned int fixed_reg = reg;// % 128;
	int ret;

	//return i2c_smbus_read_byte_data(file, fixed_reg & 0xff);

	unsigned char data[2];
	data[0] = fixed_reg & 0xff;
	write(file, data, 1);
    read(file, data, 1);
	return data[0];
}

unsigned int crc_table[256];

static unsigned int crc( const unsigned char *msg, int size)
{
    unsigned int crc = 0xFFFFFFFF;
    int i;
    for(i = 0; i < size; ++i)
        crc = (crc >> 8) ^ crc_table[(crc ^ msg[i]) & 0xFF];
    return crc ^ 0xFFFFFFFF;
}
static void init_crc(/*gestic_t *gestic*/)
{
    int i, j;
    unsigned long crc;

    for (i = 0; i < 256; ++i) {
        crc = i;

        for (j = 0; j < 8; ++j) {
            if ( crc & 0x00000001L )
                crc = (crc >> 1) ^ CRC_P_32;
            else
                crc = crc >> 1;
        }

        crc_table[i] = crc;
    }

    //gestic->flash.crc_intialized = 1;
}

int ReceiveMsg(uint8_t* msg){
	int ret = -1;
	for(;;) {
	  if (!digitalRead(XFER)) {
		  
		pinMode(XFER_DIR, OUTPUT);
		digitalWrite(XFER, LOW);
		usleep(1000);
		unsigned char d_size,d_flags,d_seq,d_ident;
		
			read(file, msg, 200);
			d_size  = msg[0];
			d_seq  = msg[2];
			d_ident = msg[3];
			//printf("size: %x, seq: %x, ident: %x\n", d_size, d_seq, d_ident);
			if (d_size < 4) {
				ret = -1;
				printf("read error\n");
			} 
			
			ret = d_size;

		digitalWrite(XFER, HIGH);
		pinMode(XFER_DIR, INPUT);
		
		return ret;
	  }
	}
}

int send_message(uint8_t* msg, uint32_t len) {
	usleep(30000);
	int ret = write(file, msg, len);
	usleep(30000);
	if (ret < 0) {
		printf("write error\n");
		return ret;
	}
	ret = ReceiveMsg(rcvMsg);
	if (ret < 0) return ret;
	if (msg[3] == 0x15) return GET_U16(&(msg[6]));
	return 0;
}

int gestic_flash_begin(/*gestic_t *gestic, */unsigned int session_id, void *iv,
                       gestic_UpdateFunction_t mode, int timeout)
{
    //int error = GESTIC_NO_ERROR;
    unsigned char msg[28];
    //gestic_version_request_t v_request;

    /* Init table for CRC32-checksum */
    /*if(!gestic->flash.crc_intialized)*/
        init_crc();
	printf("gestic_flash_begin \n");
    /* Prepare flash start message */
    memset(msg, 0, sizeof(msg));
    SET_U8(msg, sizeof(msg));
    SET_U8(msg + 3, gestic_msg_Fw_Update_Start);
    SET_U32(msg+8, session_id);
    memcpy(msg+12, iv, 14);
    SET_U8(msg+26, mode);

    //memset(&v_request, 0, sizeof(v_request));

    SET_U32(msg+4, crc(msg+8, 20));

    gest_session_id = session_id;
	
    /* Reset device and wait for the firmware-version */
    //gestic->version_request = &v_request;
    //error = gestic_reset(gestic);
	digitalWrite(RESET, LOW);
	usleep(20000);
	digitalWrite(RESET, HIGH);
	usleep(50000);

    /*if(!error)
        error = gestic_wait_for_version_info(gestic, timeout);
    gestic->version_request = 0;*/

	int rcv = ReceiveMsg(rcvMsg);
	printf("ReceiveMsg \n");
	if (rcv >= 0) {
		rcvMsg[rcv] = '\0';
		printf("%s \n", (char*)(&(rcvMsg[4])));
	}
	

    /* Send message to start flash-mode */
    /*if(!error)
        error = gestic_send_message(gestic, msg, sizeof(msg), timeout);*/
	int ret = send_message(msg, sizeof(msg));//write(file, msg, sizeof(msg));
	usleep(100000);
	//printf("gestic_flash_begin ret %d \n",ret);

    return 0;
}

int gestic_flash_write(unsigned short address,
                       unsigned char length, unsigned char *record,
                       gestic_UpdateFunction_t mode, int timeout)
{
    unsigned char msg[140];
	
    /* Assert that only verification is used for verification sessions */
    GESTIC_ASSERT(session_mode != gestic_UpdateFunction_VerifyOnly ||
            mode == gestic_UpdateFunction_VerifyOnly);

    memset(msg, 0, sizeof(msg));
    SET_U8(msg, sizeof(msg));
    SET_U8(msg + 3, gestic_msg_Fw_Update_Block);
    SET_U16(msg + 8, address);
    SET_U8(msg + 10, length);
    SET_U8(msg + 11, mode);
    memcpy(msg + 12, record, 128);

    SET_U32(msg + 4, crc(msg + 8, 132));

	int ret = send_message(msg, sizeof(msg));//write(file, msg, sizeof(msg));
	usleep(1000);
	printf("gestic_flash_write: %s \n",ret==0?"success":"failed");
    return 0;//gestic_send_message(gestic, msg, sizeof(msg), timeout);
	
}

int gestic_flash_end(unsigned char *version, int timeout)
{
    //int error = GESTIC_NO_ERROR;
    unsigned char msg[136];
    memset(msg, 0, sizeof(msg));

    /* Finish by writing the version information */

    SET_U8(msg, sizeof(msg));
    SET_U8(msg + 3, gestic_msg_Fw_Update_Completed);
    SET_U32(msg + 8, gest_session_id);
    SET_U8(msg + 12, session_mode);
    memcpy(msg + 13, version, 120);

    SET_U32(msg + 4, crc( msg + 8, 128));
	usleep(100000);
    //error = gestic_send_message(gestic, msg, sizeof(msg), timeout);
	int ret = send_message(msg, sizeof(msg));//write(file, msg, sizeof(msg));
	usleep(100000);
	printf("gestic_flash_end ret %d \n",ret);
    /* Finally restart device */

    //if(!error) {
        SET_U8(msg + 12, gestic_UpdateFunction_Restart);
        memset(msg + 13, 0, 120);

        SET_U32(msg + 4, crc(msg + 8, 128));

        //error = gestic_send_message(gestic, msg, sizeof(msg), timeout);
		send_message(msg, sizeof(msg));//write(file, msg, sizeof(msg));
		usleep(100000);
    //}

    return 0;
}

int gestic_flash_image(
                       unsigned int session_id,
                       gestic_flash_image_t *image,
                       gestic_UpdateFunction_t mode,
                       int timeout)
{
    //int error = GESTIC_NO_ERROR;
    int i;
    gestic_flash_record_t *record;

     gestic_flash_begin(session_id, image->iv, mode, timeout);

    for(i = 0; i < image->record_count; ++i) {
		usleep(1000);
        record = image->data + i;
        gestic_flash_write(record->address, record->length,
                                   record->data, mode, timeout);
    }

    gestic_flash_end(image->fw_version, timeout);

    return 0;
}

int main(void) {

    char filename[40];
    const unsigned char  *buffer;
    int addr = 0x42;        // The I2C address of mgc

    sprintf(filename,"/dev/i2c-1");
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }

    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }
	
	//  Configure xfer direction as Output
    int export = open("/sys/class/gpio/export", O_WRONLY); 
	if(export < 0) printf("can't open  gpio export file\n");
	int ret = write(export, "27", 2);
	if(ret < 0) printf("can't export xfer pin\n");
	ret = write(export, "17", 2);
	if(ret < 0) printf("can't export reset pin\n");
	ret = close(export); 
	
	pinMode("/sys/class/gpio/gpio5/direction", "out");
	digitalWrite("/sys/class/gpio/gpio5/value", LOW);

	pinMode(XFER_DIR, INPUT/*_PULLUP*/);
	pinMode(RESET_DIR,  OUTPUT); 
	digitalWrite(RESET, LOW);
	usleep(200000);
	digitalWrite(RESET, HIGH);
	usleep(500000);
	
	init_crc();
	
	session_mode = gestic_UpdateFunction_ProgramFlash;
	
	printf(" Loader record_count %d\n", Loader.record_count);
	printf(" lib record_count %d\n", Library.record_count);
	
	printf("Flashing loader.\n");
    gestic_flash_image(1, &Loader, gestic_UpdateFunction_ProgramFlash, 0);
	usleep(500000);
	
	printf("Flashing library.\n");
    gestic_flash_image(1, &Library, gestic_UpdateFunction_ProgramFlash, 0);

return 0;
}