/*
 UNIVERSIDADE ESTADUAL DO PIAUÍ - UESPI
 Bac. em Ciências da Computação - CTU

 AJ Alves.

 aj.alves@live.com;
 aj.zerokol@gmail.com;

 zerokol.com
 */

/********************************************************************
 * INCLUDES
 ********************************************************************/
#include "Continuous2Wheels.h"

/*
 speed* must be higher or equal to 0 and lower or equal to 90
 distance** is a value in centimeter
 degree*** is a value positive or negative different of 0, the sigh determines the direction
 degree**** is a value higher then 0
 smooth***** percentage value

 Ps:. Every time you use one of the motion methods the value of _speed and _direction are dynamically changed
 */

/********************************************************************
 * IMPLEMENTATION OF PUBLIC METHODS
 ********************************************************************/
Continuous2Wheels::Continuous2Wheels(int rightWhreelPin, int leftWhreelPin,
		double wheellRadius, double bendRadius) {
	//Start the serial communication but with the debug mode off
	_serialPortBaud = SERIAL_BAUD_DEFAULT;
	Serial.begin(_serialPortBaud);
	_debug = false;
	//attaching referred pins to associative wheels
	_rightWheel.attach(rightWhreelPin);
	_leftWheel.attach(leftWhreelPin);
	//Setting the initial values
	_wheelRadius = wheellRadius;
	_bendRadius = bendRadius;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
	_direction = IDLE;
	//Setting the resistance value
	_resistence = RESISTENCE_DEFAULT;
	//Setting the bend smooth
	_bendSmooth = BEND_SMOOTH_DEFAULT;
}

Continuous2Wheels::Continuous2Wheels(int rightWhreelPin, int leftWhreelPin,
		double wheellRadius, double bendRadius, double resistence) {
	//Start the serial communication but with the debug mode off
	_serialPortBaud = SERIAL_BAUD_DEFAULT;
	Serial.begin(_serialPortBaud);
	_debug = false;
	//attaching referred pins to associative wheels
	_rightWheel.attach(rightWhreelPin);
	_leftWheel.attach(leftWhreelPin);
	//Setting the initial values
	_wheelRadius = wheellRadius;
	_bendRadius = bendRadius;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
	_direction = IDLE;
	//Setting the resistance value
	_resistence = resistence;
	//Setting the bend smooth
	_bendSmooth = BEND_SMOOTH_DEFAULT;
}

//Method to stop the machine
void Continuous2Wheels::stop() {
	_direction = IDLE;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
}

//Method to move the machine forward in a certain speed*
void Continuous2Wheels::forward(int speed) {
	speed = speedFormater(speed);
	_direction = FORWARD;
	_rightWheel.write(STOPED + speed);
	_leftWheel.write(STOPED - speed);
	_speed = speed;
}

//Method to move the machine forward in a certain speed* to a distance*
void Continuous2Wheels::forward(int speed, double distance) {
	speed = speedFormater(speed);
	//get the walk value of wheel displacement
	double displacement = getWalkDisplacement(distance);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//start the motion
	_rightWheel.write(STOPED + speed);
	_leftWheel.write(STOPED - speed);
	//wait until to reach the target
	delay(waitValue);
	//stop the machine
	stop();
	if (_debug) {
		Serial.print("backward(int speed, double distance) -> Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to move the machine backward in a certain speed*
void Continuous2Wheels::backward(int speed) {
	speed = speedFormater(speed);
	_direction = BACKWARD;
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	_speed = speed;
}

//Method to move the machine backward in a certain speed* to a distance*
void Continuous2Wheels::backward(int speed, double distance) {
	speed = speedFormater(speed);
	//get the walk value of wheel displacement
	double displacement = getWalkDisplacement(distance);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//start the motion
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	//wait until to reach the target
	delay(waitValue);
	//stop the machine
	stop();
	if (_debug) {
		Serial.print("backward(int speed, double distance) -> Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to make a bend with the passed degree*** value, the degree sigh will determine the bend side
/*
 Bends does not stop the machine, it gets the current speed value and make a appropriate bend
 */
void Continuous2Wheels::bend(int degree) {
	if (degree > 0) {
		rigthBend(degree);
	} else if (degree < 0) {
		degree *= -1;
		leftBend(degree);
	}
	if (_debug) {
		Serial.print("bend(int degree) -> Radians: ");
		Serial.println(degree);
	}
}

//Method to make a right bend with the passed degree**** value
/*
 Bends does not stop the machine, it gets the current speed value and make a appropriate bend
 */
void Continuous2Wheels::rigthBend(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	//Get the raidians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement
	double displacement = getCircleDisplacement(radiansVal, _bendRadius);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, _speed);
	//adjust the right wheel for a bend
	_rightWheel.write(STOPED + (_speed * _bendSmooth));
	//wait until to reach the target
	delay(waitValue);
	//restart the motion
	_rightWheel.write(getWheelSpeed(_speed, RIGHTWHEEL));
	if (_debug) {
		Serial.print("rigthBend(int degree) -> Radians: ");
		Serial.print(radiansVal);
		Serial.print(", Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to make a left bend with the passed degree**** value
/*
 Bends does not stop the machine, it gets the current speed value and make a appropriate bend
 */
void Continuous2Wheels::leftBend(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	//Get the raidians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement
	double displacement = getCircleDisplacement(radiansVal, _bendRadius);
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, _speed);
	//adjust the left wheel for a bend
	_leftWheel.write(STOPED - (_speed * _bendSmooth));
	//wait until to reach the target
	delay(waitValue);
	//restart the motion
	_leftWheel.write(getWheelSpeed(_speed, LEFTWHEEL));
	if (_debug) {
		Serial.print("leftBend(int degree) -> Radians: ");
		Serial.print(radiansVal);
		Serial.print(", Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to spin the machine in its own axis, the sign of param will determine the spin direction
void Continuous2Wheels::spin(int degree) {
	//Get the raidians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement now using the bend radius, it will make the machine spin in itself
	double displacement = getCircleDisplacement(radiansVal, (_bendRadius / 2));
	//by defaul the spin is made with a half maximum speed
	int speed = STOPED / 2;
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//from the sign of passed param choice the right direction
	if (degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	//wait until to reach the target
	delay(waitValue);
	//STOP THE MACHINE - FOR THE TIME BEING
	stop();
	if (_debug) {
		Serial.print("spin(int degree) -> Radians: ");
		Serial.print(radiansVal);
		Serial.print(", Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to spin the machine in its own axis, the sign of param will determine the spin direction and in a certain speed*
void Continuous2Wheels::spin(int degree, int speed) {
	speed = speedFormater(speed);
	//Get the raidians value from the degree
	double radiansVal = degreeToRadian(degree);
	//get the walk value of wheel displacement now using the bend radius, it will make the machine spin in itself
	double displacement = getCircleDisplacement(radiansVal, (_bendRadius / 2));
	//get the among of time to wait to reach the displacement
	unsigned long waitValue = getWaitValue(displacement, speed);
	//from the sign of passed param choice the right direction
	if (degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	//wait until to reach the target
	delay(waitValue);
	//STOP THE MACHINE - FOR THE TIME BEING
	stop();
	if (_debug) {
		Serial.print("spin(int degree, int speed) -> Radians: ");
		Serial.print(radiansVal);
		Serial.print(", Displacement: ");
		Serial.print(displacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

//Method to get the Debug mode value
bool Continuous2Wheels::getDebugMode() {
	return _debug;
}

//Method to set the Debug Mode of this lib
void Continuous2Wheels::setDebugMode(bool mode) {
	if (mode) {
		Serial.println("Debug mode is ON!");
	} else {
		Serial.println("Debug mode is OFF!");
	}
	_debug = mode;
}

//Method to get the speed value
int Continuous2Wheels::getSpeed() {
	return _speed;
}

//Method to get direction value
int Continuous2Wheels::getDirection() {
	return _direction;
}

//Method to get the bend smooth value
float Continuous2Wheels::getBendSmooth() {
	return _bendSmooth;
}

//Method to set the bend smooth***** value
void Continuous2Wheels::setBendSmooth(float smooth) {
	if (_debug) {
		Serial.print("Smooth changed to ");
		Serial.println(smooth);
	}
	if(smooth < 0.0){
		smooth = 0.0;
	}else if(smooth > 1.0){
		smooth = 1.0;
	}
	_bendSmooth = smooth;
}

//Method to get the baud value of serial port communication
long Continuous2Wheels::getSerialPortBaud() {
	return _serialPortBaud;
}

//Method to set the baud value of serial port communication
void Continuous2Wheels::setSerialPortBaud(long baud) {
	if (_debug) {
		Serial.print("Serial baud has changed to ");
		Serial.println(baud);
	}
	_serialPortBaud = baud;
	Serial.end();
	Serial.begin(_serialPortBaud);
}

/********************************************************************
 * IMPLEMENTATION OF PRIVATE METHODS
 ********************************************************************/
//Private method used to calculate the speed value to a specific wheel
int Continuous2Wheels::getWheelSpeed(int speed, int wheel) {
	switch (_direction) {
	default:
	case IDLE:
		return STOPED;
		break;
	case FORWARD:
		if (wheel == RIGHTWHEEL) {
			return STOPED + speed;
		} else {
			return STOPED - speed;
		}
		break;
	case BACKWARD:
		if (wheel == RIGHTWHEEL) {
			return STOPED - speed;
		} else {
			return STOPED + speed;
		}
		break;
	}
}

//Method to convert degrees to radians
double Continuous2Wheels::degreeToRadian(int degree) {
	return (RAD * degree);
}

//Method to calculate the circle "for wheels" displacement
double Continuous2Wheels::getCircleDisplacement(double radiansV, double radius) {
	return (radiansV * radius);
}

//Method to calculate the machine distance displacement
double Continuous2Wheels::getWalkDisplacement(double distance) {
	unsigned int degree = (360 * distance) / circleLength(_wheelRadius);
	double radiansV = degreeToRadian(degree);
	if (_debug) {
		Serial.print("getWalkDisplacement() -> Degree: ");
		Serial.print(degree);
		Serial.print(", Length: ");
		Serial.print(circleLength(_wheelRadius));
		Serial.print(", Radians: ");
		Serial.println(radiansV);
	}
	return getCircleDisplacement(radiansV, _wheelRadius);
}

//Method to calculate the length  for an circle
double Continuous2Wheels::circleLength(double radius) {
	return 2 * PI * radius;
}

//Method to calculate among of time to wait until the displacement is walked
unsigned long Continuous2Wheels::getWaitValue(double displacement, int speed) {
	return ((((displacement / speed) * MILI) * _resistence) * (1 + _bendSmooth));
}

//Method to format the speed to a valid speed value
int Continuous2Wheels::speedFormater(int speed) {
	if (speed < 0) {
		speed = 0;
	} else if (speed > 90) {
		speed = 90;
	}
	return speed;
}
