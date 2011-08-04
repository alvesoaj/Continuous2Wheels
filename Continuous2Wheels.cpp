/*
 UNIVERSIDADE ESTADUAL DO PIAUÍ - UESPI

 AJ Alves.

 aj.alves@live.com;
 aj.zerokol@gmail.com;

 zerokol.com
 */

#include "Continuous2Wheels.h"

/********************************************************************
 * IMPLEMENTATION
 ********************************************************************/

Continuous2Wheels::Continuous2Wheels(int rigthWhreelPin, int leftWhreelPin,
		double wheellRadius, double bendRadius) {
	//attaching referred pins to associative wheels
	Serial.begin(9600);
	_rightWheel.attach(rigthWhreelPin);
	_leftWheel.attach(leftWhreelPin);
	_wheelRadius = wheellRadius;
	_bendRadius = bendRadius;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
	_direction = IDLE;
	_debug = false;
}

Continuous2Wheels::Continuous2Wheels(int rigthWhreelPin, int leftWhreelPin,
		double wheellRadius, double bendRadius, double resistence) {
	Serial.begin(9600);
	_rightWheel.attach(rigthWhreelPin);
	_leftWheel.attach(leftWhreelPin);
	_wheelRadius = wheellRadius;
	_bendRadius = bendRadius;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
	_direction = IDLE;
	_resistence = resistence;
	_debug = false;
}

void Continuous2Wheels::stopWheels() {
	_direction = IDLE;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
}

void Continuous2Wheels::forward(int speed) {
	_direction = FORWARD;
	_rightWheel.write(STOPED + speed);
	_leftWheel.write(STOPED - speed);
	_speed = speed;
}

void Continuous2Wheels::forward(int speed, double distance) {
	_rightWheel.write(STOPED + speed);
	_leftWheel.write(STOPED - speed);
	double desplacement = getWalkDesplacement(distance);
	unsigned long waitValue = getWaitValue(desplacement, speed);
	delay(waitValue);
	stopWheels();
	if (_debug) {
		Serial.print("backward(int speed, double distance) -> Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

void Continuous2Wheels::backward(int speed) {
	_direction = BACKWARD;
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	_speed = speed;
}

void Continuous2Wheels::backward(int speed, double distance) {
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	double desplacement = getWalkDesplacement(distance);
	unsigned long waitValue = getWaitValue(desplacement, speed);
	delay(waitValue);
	stopWheels();
	if (_debug) {
		Serial.print("backward(int speed, double distance) -> Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

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

void Continuous2Wheels::rigthBend(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	double radiansV = degreeToRadian(degree);
	double desplacement = getDesplacement(radiansV, _bendRadius);
	unsigned long waitValue = getWaitValue(desplacement, _speed);
	_rightWheel.write(STOPED);
	delay(waitValue);
	_rightWheel.write(getSpeed(_speed, RIGTHWHEEL));
	if (_debug) {
		Serial.print("rigthBend(int degree) -> Radians: ");
		Serial.print(radiansV);
		Serial.print(", Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

void Continuous2Wheels::leftBend(int degree) {
	if (degree < 0) {
		degree = 0;
	}
	double radiansV = degreeToRadian(degree);
	double desplacement = getDesplacement(radiansV, _bendRadius);
	unsigned long waitValue = getWaitValue(desplacement, _speed);
	_leftWheel.write(STOPED);
	delay(waitValue);
	_leftWheel.write(getSpeed(_speed, LEFTWHEEL));
	if (_debug) {
		Serial.print("leftBend(int degree) -> Radians: ");
		Serial.print(radiansV);
		Serial.print(", Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

void Continuous2Wheels::spin(int degree) {
	double radiansV = degreeToRadian(degree);
	double desplacement = getDesplacement(radiansV, (_bendRadius / 2));
	if (desplacement < 0) {
		desplacement = desplacement * -1;
	}
	int speed = STOPED / 2;
	unsigned long waitValue = getWaitValue(desplacement, speed);
	if (degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	delay(waitValue);
	stopWheels();
	if (_debug) {
		Serial.print("spin(int degree) -> Radians: ");
		Serial.print(radiansV);
		Serial.print(", Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

void Continuous2Wheels::spin(int degree, int speed) {
	double radiansV = degreeToRadian(degree);
	double desplacement = getDesplacement(radiansV, (_bendRadius / 2));
	if (desplacement < 0) {
		desplacement = desplacement * -1;
	}
	unsigned long waitValue = getWaitValue(desplacement, speed);
	if (degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	delay(waitValue);
	stopWheels();
	if (_debug) {
		Serial.print("spin(int degree, int speed) -> Radians: ");
		Serial.print(radiansV);
		Serial.print(", Displacement: ");
		Serial.print(desplacement);
		Serial.print(", Waiting: ");
		Serial.println(waitValue);
	}
}

void Continuous2Wheels::setDebugMode(bool mode) {
	if (mode) {
		Serial.println("Debug mode is ON!");
	} else {
		Serial.println("Debug mode is OFF!");
	}
	_debug = mode;
}

int Continuous2Wheels::getSpeed(int speed, int wheel) {
	switch (_direction) {
	default:
	case IDLE:
		return STOPED;
		break;
	case FORWARD:
		if (wheel == RIGTHWHEEL) {
			return STOPED + speed;
		} else {
			return STOPED - speed;
		}
		break;
	case BACKWARD:
		if (wheel == RIGTHWHEEL) {
			return STOPED - speed;
		} else {
			return STOPED + speed;
		}
		break;
	}
}

double Continuous2Wheels::degreeToRadian(int degree) {
	return (RAD * degree);
}

double Continuous2Wheels::getDesplacement(double radiansV, double radius) {
	return (radiansV * radius);
}

double Continuous2Wheels::getWalkDesplacement(double distance) {
	unsigned int degree = (360 * distance) / compriment(_wheelRadius);
	double radiansV = degreeToRadian(degree);
	if (_debug) {
		Serial.print("getWalkDesplacement() -> Degree: ");
		Serial.print(degree);
		Serial.print(", Compriment: ");
		Serial.print(compriment(_wheelRadius));
		Serial.print(", Radians: ");
		Serial.println(radiansV);
	}
	return getDesplacement(radiansV, _wheelRadius);
}

double Continuous2Wheels::compriment(double radius) {
	return 2 * PI * radius;
}

unsigned long Continuous2Wheels::getWaitValue(double desplacement, int speed) {
	return (((desplacement / speed) * MILI) * _resistence);
}
