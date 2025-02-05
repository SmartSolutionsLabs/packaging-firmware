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
			if (key < 0) {
				// Change to speed configuration
				this->screen = SPEED;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
		case SPEED:
			if (key < 0) {
				// Change to delay configuration
				this->screen = DELAY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
		case DELAY:
			if (key < 0) {
				// Change to ready screen
				this->screen = READY;
				this->display->print(this->screen, this->speed, this->delay);
			}
			break;
	}
}

void Machinist::connect(void * data) {
	this->preferences = new Preferences();
	this->preferences->begin("global", false);

	// loading "constants" data
	this->speed = this->preferences->getFloat("speedStep", 1.0);
	this->delay = this->preferences->getFloat("delayStep", 1.0);
}

void Machinist::run(void* data) {
	while (1) {
		vTaskDelay(3 * this->iterationDelay);
	}
}

void Machinist::work(){
	Serial.print("I will work...\n");

	// Wake up the motor
	this->motor->moveSteps(this->speed, 1.0, 1);
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
