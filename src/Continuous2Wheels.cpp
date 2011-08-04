/*
 UNIVERSIDADE ESTADUAL DO PIAUÍ - UESPI

 AJ Alves.

 aj.alves@live.com;
 aj.zerokol@gmail.com;

 zerokol.com
 */

/********************************************************************
 * IMPLEMENTATION
 ********************************************************************/

Continuous2Wheels::Continuous2Wheels(int rigthWhreelPin, int leftWhreelPin,
		double wheellRadius, double bendRadius) {
	//attaching referred pins to associatives whreels
	_rightWheel.attach(rigthWhreelPin);
	_leftWheel.attach(leftWhreelPin);
	_wheelRadius = wheellRadius;
	_bendRadius = bendRadius;
	_rightWheel.write(STOPED);
	_leftWheel.write(STOPED);
	_speed = STOPED;
	_direction = IDLE;
}

void Continuous2Wheels::stop() {
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

void Continuous2Wheels::forward(int speed, int distance) {
	_rightWheel.write(STOPED + speed);
	_leftWheel.write(STOPED - speed);
	delay( time);
	Continuous2Wheels::stop();
}

void Continuous2Wheels::backward(int speed) {
	_direction = BACKWARD;
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	_speed = speed;
}

void Continuous2Wheels::backward(int speed, int distance) {
	_rightWheel.write(STOPED - speed);
	_leftWheel.write(STOPED + speed);
	delay( time);
	Continuous2Wheels::stop();
}

void Continuous2Wheels::rigthBend(int degree) {
	double radians = Continuous2Wheels::degreeToRadian(degree);
	double desplacement = Continuous2Wheels::getDesplacement(radians,
			_bendRadius);
	unsigned long waitValue =
			Continuous2Wheels::waitValue(desplacement, _speed);
	_rightWheel.write(STOPED);
	delay(waitValue);
	_rightWheel.write(getSpeed(_speed, RIGTHWHEEL));
}

void Continuous2Wheels::leftBend(int degree) {
	double radians = Continuous2Wheels::degreeToRadian(degree);
	double desplacement = Continuous2Wheels::getDesplacement(radians,
			_bendRadius);
	unsigned long waitValue =
			Continuous2Wheels::waitValue(desplacement, _speed);
	_leftWheel.write(STOPED);
	delay(waitValue);
	_leftWheel.write(getSpeed(_speed, LEFTWHEEL));
}

void Continuous2Wheels::spin(int degree) {
	double radians = Continuous2Wheels::degreeToRadian(degree);
	double desplacement = Continuous2Wheels::getDesplacement(radians, (radius / 2) );
	if (desplacement < 0) {
		desplacement = desplacement * -1;
	}
	int speed = STOPED / 2;
	unsigned long waitValue = Continuous2Wheels::waitValue(desplacement, speed);
	if(degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else if {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	delay(waitValue);
	Continuous2Wheels::stop();
}

void Continuous2Wheels::spin(int degree, int speed) {
	double radians = Continuous2Wheels::degreeToRadian(degree);
	double desplacement = Continuous2Wheels::getDesplacement(radians, (radius / 2) );
	if (desplacement < 0) {
		desplacement = desplacement * -1;
	}
	unsigned long waitValue = Continuous2Wheels::waitValue(desplacement, speed);
	if(degree < 0) {
		_rightWheel.write(speed);
		_leftWheel.write(speed * -1);
	} else if {
		_rightWheel.write(speed * -1);
		_leftWheel.write(speed);
	}
	delay( waitValue);
	Continuous2Wheels::stop();
}

int Continuous2Wheels::getSpeed(int speed, int wheel) {
	switch (_direction) {
	default:
	case IDLE:
		return STOPED;
	case FORWARD:
		if (wheel == RIGTHWHEEL) {
			return STOPED + speed;
		} else {
			return STOPED - speed;
		}
	case BACKWARD:
		if (wheel == RIGTHWHEEL) {
			return STOPED - speed;
		} else {
			return STOPED + speed;
		}
	}
}

double Continuous2Wheels::degreeToRadian(int degree) {
	return (RAD * degree);
}

double Continuous2Wheels::getDesplacement(double radians, double radius) {
	return (radians * radius);
}

unsigned long Continuous2Wheels::getWaitValue(double desplacement, int speed) {
	return ((desplacement / speed) * 10000);
}
