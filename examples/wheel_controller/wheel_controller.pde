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
double resistance = 2.6; //a estimated resistance for the middle in the wheels
Continuous2Wheels c2w(&rightWheel, &leftWheel, wheelRadius, bendRadius, resistance); //Creating a instance for our lib and setting the constructor

void setup(){
  rightWheel.attach(rightWheelPin);
  leftWheel.attach(leftWheelPin);
  // c2w.setDebugMode(true); //setting the debug mode for true to show real time logs
  // c2w.setBendSmooth(0.2); //Applying a smooth to bends
}

void loop(){
  c2w.forward(45);
  delay(4000);
  c2w.stop();
  delay(10000);
}