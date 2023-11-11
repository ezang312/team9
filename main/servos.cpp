#include "servos.h"
/* 
servo.attach(pin, min, max)
delay(millisec) sets the time that the servo runs/stalls for
servo.write() set a speed and direction for your motors, 1000 = maximum speed in one direction, and 2000 =maximum speed in the other direction.
*/
    Servo servoRight;
    Servo servoLeft;
    Servo servoCap;

void servoSetup() {
    servoRight.setPeriodHertz(50);
    servoRight.attach(13, 1000, 2000);
    servoLeft.setPeriodHertz(50);
    servoLeft.attach(14, 1000, 2000);
    servoCap.setPeriodHertz(50);
    servoCap.attach(4, 1000, 2000);
}

// Both wheels go forward
void forwards(int duration) {
    servoRight.write(1400); //clockwise, cw, "forward"
    servoLeft.write(1400); //counterclockwise, ccw, "forwards"
    delay(duration);
}

// super slow forwards
void slowForwards(int duration) {
    servoRight.write(1425); //clockwise, cw, "forward"
    servoLeft.write(1425); //counterclockwise, ccw, "forwards"
    delay(duration);
}

// Both wheels go backward
void backwards(int duration) {
    servoRight.write(1750);
    servoLeft.write(1750);
    delay(duration);
}

// Right turn
void rightTurn(int duration) {
    servoRight.write(1750); //counterclockwise, ccw, "backward"
    servoLeft.write(1250); //counterclockwise, ccw, "forward"
    delay(duration);
}


// Left turn
void leftTurn(int duration) {
    servoRight.write(1250); //clockwise, cw, "forward"
    servoLeft.write(1750); //clockwise, cw, "backward"
    delay(duration);
}

// slight adjustment to one wheel
void adjust(String direction) {
    if (direction == "left") {
        servoRight.write(1375); // Move slighty to the left
        delay(50);
    } else if (direction == "right") {
        servoLeft.write(1375); // Move slighty to the right
        delay(50);
    } else {
        Serial.println("invalid input");
    }
}

// stop
void stop(int duration) {
    servoRight.write(1500);
    servoLeft.write(1500);
    delay(duration);
}

//Shoot
void shoot(int duration) {
    servoCap.write(1400);
    delay(duration);
    servoCap.write(1500);
    delay(1000);
}