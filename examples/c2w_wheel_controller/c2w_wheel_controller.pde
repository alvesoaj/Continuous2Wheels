/*
   UNIVERSIDADE ESTADUAL DO PIAUÍ - UESPI
   Bac. em Ciências da Computação - CTU

   AJ Alves.

   aj.alves@live.com;
   aj.zerokol@gmail.com;

   zerokol.com
 */

/********************************************************************
 * EXAMPLE
 ********************************************************************/
 
#include <Servo.h>
#include <Continuous2Wheels.h> //Importing our Library

Servo rightWheel, leftWheel;
int rightWheelPin = 9; //Pin for right wheel
int leftWheelPin = 10; //Pin for left wheel
double wheelRadius = 3.6; //Radius for wheels in centimeters
double bendRadius = 12.2; //Bend radius is the distance for one wheel to another, it is the shaft length, in centimeters
// Creating a instance for our lib and setting the constructor
// Don't forget to pass a POINTER for right and left Servo objects
Continuous2Wheels c2w(&rightWheel, &leftWheel, wheelRadius, bendRadius);

void setup(){
  // Configure your Servos normally
  rightWheel.attach(rightWheelPin);
  leftWheel.attach(leftWheelPin);
}

void loop(){
  c2w.forward(45); // Move the robot forward speed of 45
  delay(4000);
  c2w.bend(90); // Make a right bent with 90°
  delay(4000);
  c2w.backward(45); // Move the robot backward speed of 45
  delay(4000);
  c2w.bend(-90); // Make a left bent with 90°
  delay(4000);
  c2w.stop(); // Stop the robot wheels
  delay(10000);
}