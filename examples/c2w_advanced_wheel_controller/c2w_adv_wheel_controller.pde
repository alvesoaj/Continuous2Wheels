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
// Creating a instance for our lib and setting the constructor
// Don't forget to pass a POINTER for right and left Servo objects
Continuous2Wheels c2w(&rightWheel, &leftWheel, wheelRadius, bendRadius, resistance);

void setup(){
  rightWheel.attach(rightWheelPin);
  leftWheel.attach(leftWheelPin);
  c2w.setBendSmooth(0.2); //Applying a smooth to bends
}

void loop(){
  c2w.forward(45);
  delay(4000);
  c2w.backward(45);
  delay(4000);
  c2w.bend(-90); // Make a left bent with 90°
  delay(4000);
  c2w.bend(-90); // Make a left bent with 90°
  delay(4000);
  c2w.bend(-90); // Make a left bent with 90°
  delay(4000);
  c2w.bend(-90); // Make a left bent with 90°
  delay(4000);
  c2w.spin(360); // Spin the robot 360° in its center, PS:. obstrusive, the robot stop in the end
  c2w.forward(45);
  delay(4000);
  c2w.stop();
  delay(10000);
}