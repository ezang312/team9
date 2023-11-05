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

void colorSetup() {
    //I2C protocol
    I2C_0.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    //colour sensor setup
    apds.setInterruptPin(APDS9960_INT);
    apds.begin();
    Serial.begin(115200);
}

void color() {
    int r, g, b, a; //How much red, green, and blue + (ambiguous)
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
    
}
