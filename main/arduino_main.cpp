/****************************************************************************
Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>
#include <ArduinoController.h>

#include <ESP32Servo.h>
// for Distance sensor
#include <ESP32SharpIR.h>
// #include <QTRSensors.h>

// For Servos
#include "servos.h"

// For maze, line, color
#include "maze.h"
#include "line.h"
#include "color.h"

// New Changes
#define LED 2 // ONBOARD LED

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
bool testedGamepadConnection = false;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == nullptr) {
            myGamepads[i] = gp;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        // Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
    }
}

void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            // Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        // Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

ESP32SharpIR sensor1( ESP32SharpIR::GP2Y0A21YK0F, 27);

// Arduino setup function. Runs in CPU 1
void setup() {
    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // should be called when  user performs a "device factory reset", or similar
    // prevents "paired" gamepads to reconnect but might also fix connection issues
    BP32.forgetBluetoothKeys();

    // lights up LED
    pinMode(LED, OUTPUT);

    // ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);
    
    servoSetup();
    lineSetup();
    mazeSetup();
    colorSetup();

    
    Serial.println("we have set up servos, line follow, maze, and color");
    
}

Controller controller;

// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected() && !testedGamepadConnection) {

            Serial.println("Gamepad connected!");

            // servo.write( ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );
            
            // Another way to query the buttons, is by calling buttons(), or
            // miscButtons() which return a bitmask.
            Serial.printf(
                "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
                "%4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
                i,                        // Gamepad Index
                myGamepad->dpad(),        // DPAD
                myGamepad->buttons(),     // bitmask of pressed buttons
                myGamepad->axisX(),       // (-511 - 512) left X Axis
                myGamepad->axisY(),       // (-511 - 512) left Y axis
                myGamepad->axisRX(),      // (-511 - 512) right X axis
                myGamepad->axisRY(),      // (-511 - 512) right Y axis
                myGamepad->brake(),       // (0 - 1023): brake button
                myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
                myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
            );

            // You can query the axis and other properties as well. See Gamepad.h
            // For all the available functions.
        }

        testedGamepadConnection = true;
    }

    navigateMaze();

    // digitalWrite(LED, HIGH); // LED is on
    // delay(1000);
    // digitalWrite(LED, LOW); // LED is off
    // delay(1000);

    // bool aPressed = controller.a();
    // bool bPressed = controller.b();
    // bool xPressed = controller.x();

    // while (aPressed) {
    //     Serial.println("Doing color now");
    //     color();

    //     // check to see if other buttons have been pressed
    //     bPressed = controller.b();
    //     xPressed = controller.x();

    //     if (bPressed || xPressed) {
    //         aPressed = false;
    //     }
    // }
    
    // while (bPressed) {
    //     Serial.println("Navigating the maze now");
    //     navigateMaze();

    //     // check to see if other buttons have been pressed
    //     aPressed = controller.a();
    //     xPressed = controller.x();

    //     if (aPressed || xPressed) {
    //         xPressed = false;
    //     }
    // } 
    
    // while (xPressed) {
    //     // line sensor
    //     Serial.println("Following the line now");
    //     followLine();
    //     // check to see if other buttons have been pressed
    //     aPressed = controller.a();
    //     bPressed = controller.b();

    //     if (aPressed || bPressed) {
    //         xPressed = false;
    //     }
    // }


    vTaskDelay(1);
}

// Template for passing in by reference
// bool APDS9960::readColor(int& r, int& g, int& b, int& c) {
//   uint16_t colors[4];

//   if (!readCDATAL((uint8_t *)colors, sizeof(colors))) {
//     r = -1;
//     g = -1;
//     b = -1;
//     c = -1;

//     return false;
//   }

//   c = colors[0];
//   r = colors[1];
//   g = colors[2];
//   b = colors[3];

//   disableColor();

//   return true;
// }
