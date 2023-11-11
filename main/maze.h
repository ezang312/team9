#ifndef _MAZE_H_
#define _MAZE_H_

#include <Arduino.h>
#include <ArduinoGamepad.h>

void mazeSetup();
void navigateMaze(GamepadPtr controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed);


#endif