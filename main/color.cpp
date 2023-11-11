// Red: one flash
// Green: two flashes
// Blue: three flashes 
// Then test the next three strips to find one that matches the first strip.

#include <Wire.h>
#include <Arduino.h>
#include <Arduino_APDS9960.h>
#include <iostream>
#include "color.h"
#include "servos.h"


//Colour definitions
#define APDS9960_INT 2
#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_FREQ 100000

#define LED 2
int firstColor = 0;
int secondColor = 0;

TwoWire I2C_0 = TwoWire(0);
APDS9960 apds = APDS9960(I2C_0, APDS9960_INT);
bool firstColorFound = false;

void colorSetup() {
    //I2C protocol
    I2C_0.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    //colour sensor setup
    apds.setInterruptPin(APDS9960_INT);
    apds.begin();
    Serial.begin(115200);
}

void color(GamepadPtr controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed, bool& yButtonPressed) {
    // Eric's color code
    firstColor = colorDetected();
    Serial.printf("%d\n", firstColor);

    for (int i = 0; i < firstColor; i++){
        Serial.println("flashing\n");
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        delay(1000);
    }

    forwards(5000);
    stop(1000);

    while (firstColor != secondColor && firstColor != 0) {
        secondColor = colorDetected();
        if (secondColor == firstColor){

            stop(10000);
        }
        else {
            forwards(100);
            stop(100);
        }
    }   

    if (controller->b()) {
        bButtonPressed = true;
        aButtonPressed = false;
    }

    if (controller->x()) {
        xButtonPressed = true;
        aButtonPressed = false;
    }

    if (controller->y()) {
        yButtonPressed = true;
        aButtonPressed = false;
    }
}

int colorDetected() {
    int r,g,b,a;

    if (!apds.colorAvailable()) {
        forwards(100);
    } else {
        apds.readColor(r,g,b,a);
        Serial.println("\n\nNEW COLOR FOUND:");
        Serial.printf("RED: %d\n", r);
        Serial.printf("GREEN: %d\n", g);
        Serial.printf("BLUE: %d\n", b);
        Serial.printf("AMBIENT: %d\n\n", a);
        delay(1000);

        if (r > g && r > b) {
            Serial.println("Red");
            return 1;
        }
        if (g > r && g > b) {
            Serial.println("Green");
            return 2;
        }
        if (b > r && b > g) {
            Serial.println("Blue");
            return 3;
        }

    }
    return 0;
}