#ifndef FLICK_H
#define FLICK_H

#if defined(__linux__)
#include <cstddef>
#else
#include <Wire.h>
#endif

#include <stdint.h>

#define I2C_DEV_ADDR 0x42

enum FlickTouch_t { 
	TOUCH_SOUTH_ELECTRODE, 
	TOUCH_WEST_ELECTRODE,
	TOUCH_NORTH_ELECTRODE,
	TOUCH_EAST_ELECTRODE,
	TOUCH_CENTER_ELECTRODE,
	TAP_SOUTH_ELECTRODE,
	TAP_WEST_ELECTRODE,
	TAP_NORTH_ELECTRODE,
	TAP_EAST_ELECTRODE,
	TAP_CENTER_ELECTRODE,
	DOUBLE_TAP_SOUTH_ELECTRODE,
	DOUBLE_TAP_WEST_ELECTRODE,
	DOUBLE_TAP_NORTH_ELECTRODE,
	DOUBLE_TAP_EAST_ELECTRODE,
	DOUBLE_TAP_CENTER_ELECTRODE,
	FLICK_TOUCH_NOT_DEFINED
};

enum FlickGestureClass_t {
	GESTURE_CLASS_GARBAGE_MODEL,
	FLICK_GESTURE,
	CIRCULAR_GESTURE,
	GESTURE_CLASS_UNKNOWN
};

enum FlickGesture_t {
	NO_GESTURE,
	GESTURE_GARBAGE_MODEL,
	FLICK_WEST_TO_EAST,
	FLICK_EAST_TO_WEST,
	FLICK_SOUTH_TO_NORTH,
	FLICK_NORTH_TO_SOUTH,
	CIRCLE_CLOCKWISE,
	CIRCLE_COUNTER_CLOCKWISE
};

class Flick 
{
  public:
	Flick(uint8_t xferPin, uint8_t resetPin);
	~Flick();
    void Poll();
	int SetRuntimeParameter(uint16_t id, uint32_t arg0, uint32_t arg1);
    void (*touchCallback)(FlickTouch_t, uint16_t);
    void (*airWheelCallback)(int32_t);
    void (*gestureCallback)(FlickGesture_t, FlickGestureClass_t, bool, bool);
    void (*xyzCallback)(uint16_t, uint16_t, uint16_t);
    FlickGesture_t gesture;
	FlickTouch_t touch;
    uint16_t  x, y, z; // range [0-6536]
    int32_t angle;
  private:
	int _ReceiveMsg();
	int _SendMsg(uint8_t data[], uint8_t length);
	void _ProcessSensorData(uint8_t *buffer, const uint8_t len);
    uint8_t xfer, rst, addr;
    uint8_t _receiveData[256];
	int32_t _prevWheelAngle;
};

#endif