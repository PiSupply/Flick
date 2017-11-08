# Firmware update
_Under no circumnstances you should be required to update the firmware of your Flick board!_
The procedure described here should only be attempted if a new firmware is released and if directed by our support team

## Preparing for the update
Under `Flick/firmware` you will find versions of the firmware for the different boards. Copy the one you need under `Flick\firmware\update`
In this example we will update a Flick Large
``` bash
cd /home/pi/Flick/firmware/update
cp ../Flick_Large_V1.1.enz .
```

## Perform the update
From `/home/pi/Flick/firmware/update` run:
``` bash
./flickupdate Flick_Large_V1.1.enz
```

You should see:
```
can't open  gpio xfer direction file
can't open  pin file
can't open  gpio xfer direction file
can't open  gpio xfer direction file
can't open  pin file
 Loader record_count 109
 lib record_count 222
Flashing loader.
gestic_flash_begin
ReceiveMsg
v▒s
gestic_flash_write: success
gestic_flash_write: success
...
gestic_flash_end ret 0
Flashing library.
gestic_flash_begin
ReceiveMsg
▒4
gestic_flash_write: success
gestic_flash_write: success
...
gestic_flash_write: success
gestic_flash_end ret 0
```

## Compile the tool
The flickupdate tool is already compiled for you to use but should you work on a different version of Raspbian or a different OS entirely you can compile the tool as follow.

flickupdate.c if c source file that can be built to update firmware on Flick 3D gesture boards.

Usage on Linux:
- Unpack .enz customized firmware exported from Microchip Aurea Tool. 
- Copy Library.c and Loader.c files to same directory as flickupdate.c
- Enter source directory and run build command:
cc flickupdate.c Library.c Loader.c -o flickupdate
- Run program to write new firmware:
./flickupdate
