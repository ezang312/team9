#ifndef _SERVOS_H_
#define _SERVOS_H_

#include <Arduino.h>
#include <ESP32Servo.h>

void servoSetup();

void forwards(int duration);
void slowForwards(int duration);
void backwards(int duration);

void rightTurn(int duration);
void leftTurn(int duration);
void adjust(String direction);

void stop(int duration);
void shoot(int duration);

#endif