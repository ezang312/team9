#include "line.h"
#include "servos.h"

QTRSensors qtr;

void lineSetup() {
// Setup for line sensor
    Serial.begin(115200);
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]) {33, 32, 35}, 3);
    // callibration sequence
    for (uint8_t i = 0; i < 150; i++) {
        Serial.print("calibrating: ");
        Serial.println(i);
        qtr.calibrate();
        delay(20);
    }
    // qtr.calibrate();
}

int followLine(GamepadPtr controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed, bool& yButtonPressed) {
    uint16_t sensors[3];
    int16_t position = qtr.readLineBlack(sensors);
    Serial.print("Position: ");
    Serial.print(position);
    Serial.print(" -- ");

    if (position < 750 || position > 1250) {
        while (position < 750 || position > 1250) {
            if (position > 1250) {
                Serial.println("Black line on the right");
                adjust("right");
            } else if (position < 750) {
                Serial.println("Black line on the left");
                adjust("left");
            }

            stop(2);

            // recheck position
            position = qtr.readLineBlack(sensors);
            Serial.print("Position: ");
            Serial.print(position);
            Serial.print(" -- ");
        }
    }
    if (position >= 750 && position <= 1250) { //>= 3000 && position <= 4000
        while (position >= 750 && position <= 1250) {
            slowForwards(1);

            // recheck position
            position = qtr.readLineBlack(sensors);
            Serial.print("Position: ");
            Serial.print(position);
            Serial.print(" -- ");
            Serial.println("Black line straight Ahead"); 
        }
    }
    if (controller->a()) {
        aButtonPressed = true;
        xButtonPressed = false;
    }

    if (controller->b()) {
        bButtonPressed = true;
        xButtonPressed = false;
    }
    return 0;
}