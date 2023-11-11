#include "maze.h"
#include "servos.h"
#include <ESP32SharpIR.h>

ESP32SharpIR leftIR( ESP32SharpIR::GP2Y0A21YK0F, 39);
ESP32SharpIR rightIR( ESP32SharpIR::GP2Y0A21YK0F, 34);
ESP32SharpIR frontIR( ESP32SharpIR::GP2Y0A21YK0F, 36);


void mazeSetup() {
    leftIR.setFilterRate(0.1f);
    rightIR.setFilterRate(0.1f);
    frontIR.setFilterRate(0.1f);
}

void navigateMaze(Controller controller, bool& aButtonPressed, bool& bButtonPressed, bool& xButtonPressed) {
    // Idk what this part is for, but it's in the github
    // Serial.println(frontIR.getDistanceFloat());
    // servoRight.write(1000);
    // servoLeft.write(1250);
    // delay(100);
    // servoRight.write(1500);
    // servoLeft.write(1500);
    // delay(100);

    // Eric's Maze code:'
    Serial.println(frontIR.getDistanceFloat());
    //Serial.println(leftIR.getDistanceFloat());
    //Serial.println(rightIR.getDistanceFloat());
    forwards(250);
    if(frontIR.getDistanceFloat() < 16.0) { //If front gets too close to wall   
 
        if(leftIR.getDistanceFloat() > 20.0) { //There is open space on the left, turn left
            Serial.println("Turn Left");
            leftTurn(1100);
            stop(100);
        } else if (rightIR.getDistanceFloat() > 20.0) { //Check right side, if open space, turn right
            Serial.println("Turn Right");
            rightTurn(1100);
            stop(100);
        }
    }

    if(leftIR.getDistanceFloat() < 11.0) { //If left side gets too close to wall    
        Serial.println("Adjust left");
        adjust("left");
        stop(10);
    }

    if(rightIR.getDistanceFloat() < 11.0) { //If right side gets too close to wall    
        Serial.println("Adjust right");
        adjust("right");
        stop(10);
    }
    
    if (controller->a()) {
        aButtonPressed = true;
        bButtonPressed = false;
    }

    if (controller->x()) {
        xButtonPressed = true;
        bButtonPressed = false;
    }
}