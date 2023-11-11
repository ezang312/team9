// Red: one flash
// Green: two flashes
// Blue: three flashes 
// Then test the next three strips to find one that matches the first strip.

#include <Wire.h>
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

void color() {
    // Eric's color code
    int storedR, storedG, storedB, storedA; // How much red, green, and blue + (ambiguous) in first color detected
    int newR, newG, newB, newA; // will have values of next colors detected
    int resultFirst = 0;
    int resultSecond = 0;

    forwards(1000);
    stop(1000);

    // Get first color
    if (apds.colorAvailable() && !firstColorFound) {

        apds.readColor(storedR, storedG, storedB, storedA);

        Serial.println("\nFirst color found");
        // print color
        Serial.printf("RED: %d\n", storedR);
        Serial.printf("GREEN: %d\n", storedG);
        Serial.printf("BLUE: %d\n", storedB);
        Serial.printf("AMBIENT: %d\n", storedA);

        if (storedR > storedG + 20 && storedR > storedB + 20) {
            resultFirst = 1;
            firstColorFound = true;
        }
        if (storedG > storedR + 20 && storedG > storedB + 20) {
            resultFirst = 2;
            firstColorFound = true;
        }
        if (storedB > storedR + 20 && storedB > storedG + 20) {
            resultFirst = 3;
            firstColorFound = true;
        }
        delay(10000);

        for (int i = 0; i < resultFirst; i++) {
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
            delay(1000);
        }
    }

    // Get other colors and compare
    if (apds.colorAvailable() && firstColorFound) {

        apds.readColor(newR, newG, newB, newA);

        // print color
        Serial.println("\n\nNEW COLOR FOUND:");
        Serial.printf("RED: %d\n", newR);
        Serial.printf("GREEN: %d\n", newG);
        Serial.printf("BLUE: %d\n", newB);
        Serial.printf("AMBIENT: %d\n\n", newA);
        delay(10000);

        if (newR > newG && newR > newB) {
            resultSecond = 1;
        }
        if (newG > newR && newG > newB) {
            resultSecond = 2;
        }
        if (newB > newR && newB > newG) {
            resultSecond = 3;
        }
    }

    if (resultFirst == resultSecond && resultFirst != 0) {
        Serial.println("Color found!");
        stop(10000);
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