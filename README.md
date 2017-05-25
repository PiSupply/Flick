# Flick
Resources for Flick range

# Setup Flick
Just run the following script in a terminal window and Flick will be automatically setup.
```bash
# Run this line and Flick will be setup and installed
curl -sSL https://pisupp.ly/flickcode | sudo bash
```

# Python API (Coming soon)

# Command Line
```bash
# Run a demo script to test the various gestures
python ~/Flick/flick/flickdemo.py
```

# Hardware tips
You can find a full quick start guide at the [MakerZone Flick Quick Start and FAQ](https://www.pi-supply.com/make/flick-quick-start-faq) where we also provided a pinout diagram for all the boards.

## Flick large pin mapping 
We arranged the connectivity so that only the pins on the left side of Raspberry Pi's header are required to be connected to the Flick Large.

**Flick -> Raspberry Pi**
```bash
LED2 (Red)   -> Pin 15
LED1 (Green) -> Pin 7
GND          -> Pin 9
TS           -> Pin 13
Reset        -> Pin 11
SCL          -> Pin 5
SDA          -> Pin 3
VCC          -> Pin 1
```
controlling LEDs

# Additional information
The Flick boards use the MGC3130 3D gesture controller based on Microchip's patented Gestic© technology. You can find more [Microchips's website](http://www.microchip.com/design-centers/capacitive-touch-sensing/gestic-technology/overview)

