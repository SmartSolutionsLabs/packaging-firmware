#include "Machinist.hpp"

Machinist::Machinist(const char * name, int taskCore) : Module(name, taskCore), screen(READY) {
}

Machinist::~Machinist() {
}

void Machinist::handleArrivedFloor(unsigned int id, bool value) {
	// Bottle sensor
	if (id == 0 && this->enabled && value == false && this->flag_bottle_detected) {
		this->newBottle = true;
		Serial.println("--------------- New bottle detected in run() method. ---------------------------");
		return;
	}

	// Label sensor
	if (id == 1) {
		this->enabled = value;
		return;
	}
}

void Machinist::handlePush(int key) {
	// Analize in wich screen we are and process keys according to that
	switch (this->screen) {
		case READY:
			if (key == 1) {
				// Change to speed configuration
				this->screen = OFFSET;
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = SPEED;
			}
			else if (key == 2) {
				// There is no functionality
				return;
			}
			break;
		case SPEED:
			if (key == 1) {
				// Change to speed configuration
				this->screen = READY;
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = DELAY;
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = CHANGE_SPEED;
			}
			break;
		case DELAY:
			if (key == 1) {
				// Change to speed configuration
				this->screen = SPEED;
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = LENGTH;
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = CHANGE_DELAY;
			}
			break;

		case LENGTH:
			if (key == 1) {
				// Change to speed configuration
				this->screen = DELAY;
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = OFFSET;
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = CHANGE_LENGTH;
			}
			break;

		case CHANGE_SPEED:
			if (key == 1) {
				// Change to speed configuration
				this->setSpeed(this->speed - this->speedStep);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->setSpeed(this->speed + this->speedStep);
			}
			else if (key == 2) {
				// Saving after setting
				this->saveSpeed();
				// Change to speed configuration
				this->screen = SPEED;
			}
			break;

		case CHANGE_DELAY:
			if (key == 1) {
				// Save delay decrease
				this->setDelay(this->delay - this->delayStep);
			}
			else if (key == 3) {
				// Save delay increase
				this->setDelay(this->delay + this->delayStep);
			}
			else if (key == 2) {
				// Saving after setting
				this->saveDelay();
				// Change to delay configuration
				this->screen = DELAY;
			}
			break;

		case CHANGE_LENGTH:
			if (key == 1) {
				// Save delay decrease
				this->setLabelLength(this->labelLength - 0.005);
			}
			else if (key == 3) {
				// Save delay increase
				this->setLabelLength(this->labelLength + 0.005);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->saveLabelLength();
				this->labelLength = this->getLabelLength();
				this->screen = LENGTH;
			}
			break;

		case OFFSET:
			if (key == 2) {
				this->screen = MANUAL_MOVEMENT;
			}
			if (key == 1) {
				this->screen = LENGTH;
			}
			else if (key == 3) {
				// Save delay increase
				this->screen = READY;
			}
			break;

		case MANUAL_MOVEMENT:
			if (key == 3) {
				this->motor->testSteps(8000);
				this->motor->testSteps(this->testStep);
			}
			else if (key == 2) {
				// Go to home screen
				this->screen = OFFSET;
			}
			break;
	}

	// Always send values to display
	this->display->print(this->screen, this->speed, this->delay,this->labelLength);
}

void Machinist::connect(void * data) {
	this->preferences = new Preferences();
	this->preferences->begin("global", false);

	// loading all data set by user in menu
	this->speed = this->preferences->getFloat("speed", 100);
	this->delay = this->preferences->getFloat("delay", 450.0);

	// loading "steps" data
	this->speedStep = this->preferences->getFloat("speedStep", 0.1);
	this->delayStep = this->preferences->getFloat("delayStep", 5);

	// loading "constants" data
	this->labelLength = this->preferences->getFloat("labelLength", 64.500); // define el tamaño de etiqueta
	this->Kstepcm = this->preferences->getFloat("Kstepcm", 62.5); //define cuantos pasos da el motor para mover la etiqueta 1 cm.

	// Loading values using in tests
	this->testStep = this->preferences->getInt("testStep", 10);
}

void Machinist::run(void* data) {
	static unsigned long newBottleCounter = millis() ;
	static bool newBottleCounter_flag = false;

	while (1) {
		vTaskDelay(1);
		if(this->newBottle == true && this->motor->isWorking() == false ){
			Serial.printf(" ########### New Bottle Inserted ###########\n");
			this->newBottle = false;
			newBottleCounter = millis(); // esto lo ejecuta solamente el sensor;
			newBottleCounter_flag = true;
			this->flag_bottle_detected = false;
		}

		if( newBottleCounter_flag == true && (millis() - newBottleCounter) >= this->delay  &&  this->motor->isWorking() == false ){
			Serial.printf(" ###########    Motor Start      ###########\n");
			newBottleCounter_flag = false;
			this->flag_bottle_detected = true;
			this->work();                    // paso el "delay " y empieza a trabajar motor
		}
	}
}

void Machinist::work(){
	Serial.print("----------------- I will work ---------------\n");

	// Wake up the motor
	this->motor->moveSteps(this->speed, this->labelLength, this->Kstepcm);
}

void Machinist::test(int _steps){
	Serial.print("I will test...\n");

	// Wake up the motor
	this->motor->testSteps(_steps);
}

void Machinist::showData(){
	this->display->print(this->screen, this->speed, this->delay, this->labelLength);
}

void Machinist::setSpeed(float speed) {
	if (speed < 1.5) {
		this->speed = 1.5;
	}
	else if (speed > 300) {
		this->speed = 300;
	}
	else {
		this->speed = speed;
	}
}

void Machinist::saveSpeed() {
	this->preferences->putFloat("speed", this->speed);
}

void Machinist::setDelay(float delay) {
	if (delay < 50.0) {
		this->delay = 50.0;
	}
	else if (delay > 5000) {
		this->delay = 5000.0;
	}
	else {
		this->delay = delay;
	}
}

void Machinist::saveDelay() {
	this->preferences->putFloat("delay", this->delay);
}

void Machinist::saveSpeedStep(float step) {
	this->speedStep = step;

	this->preferences->putFloat("speedStep", this->speedStep);
}

void Machinist::saveDelayStep(float step) {
	this->delayStep = step;

	this->preferences->putFloat("delayStep", this->delayStep);
}

void Machinist::setLabelLength(float newlabelLength) {
	if (newlabelLength < 1.0) {
		this->labelLength = 1.000;
	}
	else if (newlabelLength > 1000) {
		this->labelLength = 1000.000;
	}
	else {
		this->labelLength = newlabelLength;
	}

}

void Machinist::saveLabelLength() {
	this->preferences->putFloat("labelLength", this->labelLength);
}

void Machinist::saveKstepcm(float newKstepcm) {
	this->Kstepcm = newKstepcm;

	this->preferences->putFloat("Kstepcm", this->Kstepcm);
}

void Machinist::saveTestStep(int step) {
	this->testStep = step;

	this->preferences->putInt("testStep", this->testStep);
}

void Machinist::enable(bool enabled) {
	this->enabled = enabled;
}

float Machinist::getLabelLength(){
	return this->preferences->getFloat("labelLength", 64.500); // define el tamaño de etiqueta	
}

float Machinist::getKstepcm(){
	return this->Kstepcm = this->preferences->getFloat("Kstepcm", 62.5); //define cuantos pasos da el motor para mover la etiqueta 1 cm.	
}
	