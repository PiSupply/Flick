![Alt text](https://user-images.githubusercontent.com/16068311/30544599-f0c3570e-9c7e-11e7-9f3f-74282a271749.png?raw=true "Flick Logo")
# Flick
This repository contains software and resources for the [Flick range](https://pisupp.ly/flick1) of gesture sensing and 3D tracking add on boards (and cases) for the Raspberry Pi and standalone applications (and soon BeagleBone, Arduino and more!). Brought to you by [Pi Supply](https://www.pi-supply.com)

# Setup Flick
Just run the following script in a terminal window and Flick will be automatically setup.
```bash
# Run this line and Flick will be setup and installed
curl -sSL https://pisupp.ly/flickcode | sudo bash
```

# Python API (Coming soon)

# Command Line
```bash
# Drive USB Controlled Robotic Arm Construction Kit with gesture
flick-armcontrol

# Run a demo script to test the various gestures
flick-demo

# A nice way to move a snail on the screen. Flick left, right, up or down to move @
flick-snail
 
# A volume controller based on the Airwheel gesture
flick-volctrl

# 2048 game (Requires Pimoroni Unicorn HAT)
flick-2048

# Gesture demo (Requires Raspberry Pi Sense HAT)
flick-sense
```

# Hardware tips
You can find a full quick start guide at the [Maker Zone Flick Quick Start and FAQ](https://www.pi-supply.com/make/flick-quick-start-faq) where we have also provided a pinout diagram for all of the boards.

To verify that your Flick is available on the I2C bus run
```bash
i2cdetect -y 1
```
You should get a grid like matrix where the number 0x42 represents the address of your Flick board. Should the number not be visible check that I2C has been enabled or that the cabling has been properly done in the case of the Flick Large.

Note: This does not work immediately after boot. The chip on the Flick board first has to be reset by a low pulse on GPIO 17 before i2c detection will work. If you have the gpio program installed (`sudo apt-get install wiringpi`) then use the following one-liner to reset the Flick and check its presence immediately after boot:
```bash
gpio mode 17 out; gpio write 17 0; gpio write 17 1; i2cdetect -y 1
```

## Flick Large pin mapping 
We arranged the connectivity so that only the pins on the left side of Raspberry Pi's header are required to be connected to the Flick Large.

**Flick -> Raspberry Pi**
```
LED2 (Red)   -> Pin 15
LED1 (Green) -> Pin 7
GND          -> Pin 9
TS           -> Pin 13
Reset        -> Pin 11
SCL          -> Pin 5
SDA          -> Pin 3
VCC          -> Pin 1
```
### Controlling the dual LED on Flick Large
We have provided a dual LED for additional customisable feedback on the Flick Large. The red LED is connected to Pin 15/GPIO22 whereas the green LED is connected to Pin 7/GPIO4. You can drive these LEDs programmatically or via the command line.

#### Bash
This will turn on the green LED
```bash
gpio -g mode 22 out
gpio -g write 22 1
```
This will turn off the red LED
```bash
gpio -g mode 4 out
gpio -g write 4 0
```

#### Python
This will turn off the green using RPi.GPIO
```Python
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(22, GPIO.OUT)
GPIO.output(22, True)
```

This will turn off the green LED using gpiozero
```Python
from gpiozero import LED
led = LED(22)
led.on()
```

# Additional information
The Flick boards use the MGC3130 3D gesture controller based on Microchip's patented Gestic© technology. You can find more information at [Microchips's website](http://www.microchip.com/design-centers/capacitive-touch-sensing/gestic-technology/overview)

The Flick boards can also be used with the Microchip [Aurea Software Package](http://www.microchip.com/mymicrochip/filehandler.aspx?ddocname=en565745) and the [MGC3130 Hillstar Development Kit](http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=dm160218). The Hillstar Development kit contains a USB to I2C interface board which allows you to connect the Flick boards (via a jumper cable) to a PC. The Aurea software comes with a demo application which can be used to test the boards and program new firmware. It also has a mouse emulator and a media player controller.

# Third party software libraries

It is safe to say we have an awesome and growing community of people adding gesture sensing to their projects with Flick and we get a huge amount of contributions of code, some of which we can easily integrate here and others which we can't (we are only a small team). However we want to make sure that any contributions are easy to find, for anyone looking. So here is a list of other software libraries that might be useful to you (if you have one of your own, please visit the ["Issues"](https://github.com/PiSupply/Flick/issues) tab above and let us know!):

* [Send Flick data to MQTT and Philips Hue Lamp Control](https://github.com/unixweb/Flick) and there is also an accompanying blog post [here](https://blog.unixweb.de/3d-gestensteuerung-mit-flick/)
* [Image Flick Demo Using Pygame](https://github.com/ric96/flick-image)
* [Make Your Own DIY 3D Gesture and Tracking USB MIDI Controller](https://ask.audio/articles/make-your-own-diy-3d-gesture-and-tracking-usb-midi-controller) and the related code available [here](https://macprovid.vo.llnwd.net/o43/hub/media/1143/14295/Teensy_code.zip)
* [Holographic Audio Visualizer with Motion Control](https://www.hackster.io/hackerhouse/holographic-audio-visualizer-with-motion-control-e72fee) by our good friends at Hacker House!
* [Maze Controlled by Flick Shield on Raspberry Pi](https://gist.github.com/tomvdb/251de94880ee6de8fe251955fbe4066c) by Tom Van den Bon. Video also on Tom's [Twitter](https://twitter.com/geekc0der/status/877965027000459264) and also featured on Raspberry Pi [blog](https://www.raspberrypi.org/blog/flick-marble-maze/).
