#ifndef _LINE_H_
#define _LINE_H_

#include <Arduino.h>
#include <QTRSensors.h>
#include <ArduinoGamepad.h>

void lineSetup();
int followLine(GamepadPtr controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed);

#endif