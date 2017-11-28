# Slider

Slider is a very simple puzzle game written for Windows using C and the SDL2 library.

The aim of the game is to simply re-arrange the grid back into alphabetical order by moving the blank tile. You move the blank tile by using the cursor keys on your keyboard.

The there's nothing special about the code so it should compile on any platform supported by SDL2.
To compile the game using the MinGW compiler on Windows use the following command line:

`gcc  -Wall -mwindows slider.c sdl2.a -o slider.exe`
