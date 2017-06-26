C++ source files flick.h and flick.cpp defines api class that provides programming interface for Flick 3D gesture boards.
Include flick.h and flick.cpp to your c++ project.

Depends on wiringPi library for Raspberry Pi Linux

Build example program on Linux:
cd lipcpp
g++ flick_example.cpp flick.cpp -o flick_example -lwiringPi

Run example:
./flick_example
