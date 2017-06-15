flickupdate.c if c source file that can be built to update firmware on Flick 3D gesture boards.

Usage on Linux:
- Unpack .enz customized firmware exported from Microchip Aurea Tool. 
- Copy Library.c and Loader.c files to same directory as flickupdate.c
- Enter source directory and run build command:
cc flickupdate.c Library.c Loader.c -o flickupdate
- Run program to write new firmware:
./flickupdate
