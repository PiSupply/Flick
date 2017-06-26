#include "flick.h"
#include <stdio.h>
#include <unistd.h>

void onTouchCb( FlickTouch_t touch, uint16_t period ){
	switch (touch){ 
	case TOUCH_SOUTH_ELECTRODE:
		printf("TOUCH_SOUTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TOUCH_WEST_ELECTRODE:
		printf("TOUCH_WEST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TOUCH_NORTH_ELECTRODE:
		printf("TOUCH_NORTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TOUCH_EAST_ELECTRODE:
		printf("TOUCH_EAST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TOUCH_CENTER_ELECTRODE:
		printf("TOUCH_CENTER_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TAP_SOUTH_ELECTRODE:
		printf("TAP_SOUTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TAP_WEST_ELECTRODE:
		printf("TAP_WEST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TAP_NORTH_ELECTRODE:
		printf("TAP_NORTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TAP_EAST_ELECTRODE:
		printf("TAP_EAST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case TAP_CENTER_ELECTRODE:
		printf("TAP_CENTER_ELECTRODE: %d, %d \n", touch, period);
		break;
	case DOUBLE_TAP_SOUTH_ELECTRODE:
		printf("DOUBLE_TAP_SOUTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case DOUBLE_TAP_WEST_ELECTRODE:
		printf("DOUBLE_TAP_WEST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case DOUBLE_TAP_NORTH_ELECTRODE:
		printf("DOUBLE_TAP_NORTH_ELECTRODE: %d, %d \n", touch, period);
		break;
	case DOUBLE_TAP_EAST_ELECTRODE:
		printf("DOUBLE_TAP_EAST_ELECTRODE: %d, %d \n", touch, period);
		break;
	case DOUBLE_TAP_CENTER_ELECTRODE:
		printf("DOUBLE_TAP_CENTER_ELECTRODE: %d, %d \n", touch, period);
		break;
	default:
		printf("Not defined touch: %d, %d \n", touch, period);
	}
}

void airwheelCb(int32_t a) {
	printf("Angle: %d \n", a);
}

void onGestureCb(FlickGesture_t gesture, FlickGestureClass_t gestClass, bool isEdgeFlick, bool inProgress){
	const char *g, *cl;
	switch (gesture){ 
	case GESTURE_GARBAGE_MODEL:
		g = "GARBAGE_MODEL";
		break;
	case FLICK_WEST_TO_EAST:
		g = "FLICK_WEST_TO_EAST";
		break;
	case FLICK_EAST_TO_WEST:
		g = "FLICK_EAST_TO_WEST";
		break;
	case FLICK_SOUTH_TO_NORTH:
		g = "FLICK_SOUTH_TO_NORTH";
		break;
	case FLICK_NORTH_TO_SOUTH:
		g = "FLICK_NORTH_TO_SOUTH";
		break;
	case CIRCLE_CLOCKWISE:
		g = "CIRCLE_CLOCKWISE";
		break;
	case CIRCLE_COUNTER_CLOCKWISE:
		g = "CIRCLE_COUNTER_CLOCKWISE";
		break;
	default:
		g = "NO_GESTURE";
	}
	
	switch (gestClass){ 
	case GESTURE_CLASS_GARBAGE_MODEL:
		cl = "GARBAGE_MODEL";
		break;
	case FLICK_GESTURE:
		cl = "FLICK_GESTURE";
		break;
	case CIRCULAR_GESTURE:
		cl = "CIRCULAR_GESTURE";
		break;
	default:
		cl = "UNKNOWN";
	}
	
	printf("Gesture: %s, class: %s, edge flick: %s, in progress: %s \n", g, cl, isEdgeFlick?"yes":"no", inProgress?"yes":"no" );
}

void onXYZCb(uint16_t x, uint16_t y, uint16_t z){
	printf("x: %d, y: %d, z: %d \n", x, y, z);
}

int main(void) {
	Flick flick(27, 17);
	
	flick.touchCallback = onTouchCb;
	flick.airWheelCallback = airwheelCb;
	flick.xyzCallback = onXYZCb;
	flick.gestureCallback = onGestureCb;
	usleep(250000);
	int ret = flick.SetRuntimeParameter(0x80, 0x1b, 0x1F); // disable calibration
	//printf("ret: %d \n", ret);

	while (1) {
		flick.Poll();
		usleep(5000); 
	}
	return 0;
}
