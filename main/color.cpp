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
    //Move? until colour is read
    // while (!apds.colorAvailable()) {
    //     delay(5);
    // }

    // //Reads colour from sensor
    // apds.readColor(r, g, b, a);

    // //Print color in decimal
    // Serial.print("RED: ");
    // Serial.println(r);
    // Serial.print("GREEN: ");
    // Serial.println(g);
    // Serial.print("BLUE: ");
    // Serial.println(b);
    // Serial.print("AMBIENT: ");
    // Serial.println(a);


    // PAUSE FOR TESTING
    // servoRight.write(1500);
    // servoLeft.write(1500);
    // delay(3000); // no movement for 3 seconds


    // Get first color
    if (!apds.colorAvailable() && !firstColorFound) {
        forwards(5);
    } else {
        stop(100);
        firstColorFound = true;

        apds.readColor(storedR, storedG, storedB, storedA);

        Serial.println("\nFirst color found");
        // print color
        Serial.printf("RED: %d\n", storedR);
        Serial.printf("GREEN: %d\n", storedG);
        Serial.printf("BLUE: %d\n", storedB);
        Serial.printf("AMBIENT: %d\n", storedA);
        forwards(5);
    }

    // Get other colors and compare
    if (!apds.colorAvailable() && firstColorFound) {
        forwards(5);
    } else {
        stop(100);

        apds.readColor(newR, newG, newB, newA);

        // print color
        Serial.println("\n\nNEW COLOR FOUND:");
        Serial.printf("RED: %d\n", newR);
        Serial.printf("GREEN: %d\n", newG);
        Serial.printf("BLUE: %d\n", newB);
        Serial.printf("AMBIENT: %d\n\n", newA);

        // A match has been found!
        if ((storedR == newR) && (storedG == newG) && (storedB == newB) && (storedA == newA)) {
            Serial.println("Color found!");
        }
    }

    

    
}