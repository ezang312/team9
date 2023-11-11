#ifndef _COLOR_H_
#define _COLOR_H_

#include <Arduino.h>
#include <ArduinoGamepad.h>

void colorSetup();
void color(GamepadPtr controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed, bool& yButtonPressed);
int colorDetected();

#endif