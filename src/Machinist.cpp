#include "Machinist.hpp"

Machinist::Machinist(const char * name, int taskCore) : Module(name, taskCore), screen(SPEED) {
	// defining lambda to call the private work
	this->privateAction = [this]() {
		// Since timer was execute then free its resources
		if (this->countdownHandTimer) {
			esp_timer_delete(this->countdownHandTimer);
			this->countdownHandTimer = nullptr;
		}

		this->work();
	};
}

Machinist::~Machinist() {
	if (this->countdownHandTimer) {
		esp_timer_stop(this->countdownHandTimer);
	}

	if (this->countdownHandTimer) {
		esp_timer_delete(this->countdownHandTimer);
	}
}

void Machinist::handleArrivedFloor(unsigned int floorIndex, bool value) {	
	this->work();
}

void Machinist::handlePush(int key) {
	// Analize in wich screen we are and process keys according to that
	switch (this->screen) {
		case READY:
			if (key == 1) {
				// Change to speed configuration
				this->screen = DELAY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = SPEED;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = READY;
				//this->display->print(this->screen, this->speed, this->delay);
			}
			break;
		case SPEED:
			if (key == 1) {
				// Change to speed configuration
				this->screen = READY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = DELAY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = CHANGE_SPEED;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
		case DELAY:
			if (key == 1) {
				// Change to speed configuration
				this->screen = SPEED;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->screen = READY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = CHANGE_DELAY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
		case CHANGE_SPEED:
			if (key == 1) {
				// Change to speed configuration
				this->speed = this->speed - this->speedStep;
				if(this->speed < 0.1) this->speed == 0.1;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->speed = this->speed + this->speedStep;
				if(this->speed > 10) this->speed == 10;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = SPEED;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;

		case CHANGE_DELAY:
			if (key == 1) {
				// Change to speed configuration
				this->delay = this->delay - this->delayStep;
				if(this->speed < 50) this->speed == 50;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 3) {
				// Change to speed configuration
				this->delay = this->delay + this->delayStep;
				if(this->speed > 5000) this->speed == 5000;
				this->display->print(this->screen, this->speed, this->delay);
			}
			else if (key == 2) {
				// Change to speed configuration
				this->screen = DELAY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
	}
}

void Machinist::connect(void * data) {
	this->preferences = new Preferences();
	this->preferences->begin("global", false);

	// loading "steps" data
	this->speedStep = this->preferences->getFloat("speedStep", 0.1);
	this->delayStep = this->preferences->getFloat("delayStep", 50);

	// loading "constants" data
	this->labelLength = this->preferences->getFloat("labelLength", 4); // define el tamaño de etiqueta
	this->Kstepcm = this->preferences->getFloat("Kstepcm", 100); //define cuantos pasos da el motor para mover la etiqueta 1 cm.
}

void Machinist::run(void* data) {
	while (1) {
		vTaskDelay(3 * this->iterationDelay);
	}
}

void Machinist::work(){
	Serial.print("I will work...\n");

	// Wake up the motor
	this->motor->moveSteps(this->speed, this->labelLength, this->Kstepcm);
}

void Machinist::showData(){
	this->display->print(this->screen, this->speed, this->delay);
}

void Machinist::saveSpeedStep(float step) {
	this->speedStep = step;

	this->preferences->putFloat("speedStep", this->speedStep);
}

void Machinist::saveDelayStep(float step) {
	this->delayStep = step;

	this->preferences->putFloat("delayStep", this->delayStep);
}

void Machinist::saveLabelLength(float newlabelLength) {
	this->labelLength = newlabelLength;

	this->preferences->putFloat("labelLength", this->labelLength);
}

void Machinist::saveKstepcm(float newKstepcm) {
	this->Kstepcm = newKstepcm;

	this->preferences->putFloat("Kstepcm", this->Kstepcm);
}