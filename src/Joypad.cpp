#include "Joypad.hpp"

Joypad::Joypad(const char * name, int taskCore) : Module(name, taskCore) {
	this->floorButtons[0].pin = 16;
	this->floorButtons[1].pin = 4;
	this->floorButtons[2].pin = 15;

	this->iterationDelay = 5 / portTICK_PERIOD_MS;
}

void Joypad::connect(void * data) {
	pinMode(this->floorButtons[0].pin, INPUT_PULLUP);
	pinMode(this->floorButtons[1].pin, INPUT_PULLUP);
	pinMode(this->floorButtons[2].pin, INPUT_PULLUP);
}

void Joypad::run(void* data) {
	// floor buttons quantity
	unsigned int i = (sizeof(this->floorButtons) / sizeof(*this->floorButtons));
	this->floorButtons[0].lastDebounceTime = millis();
	this->floorButtons[1].lastDebounceTime = millis();
	this->floorButtons[2].lastDebounceTime = millis();

	while (1) {
		// Verify floor button state
		for(i=0;i<3;i++) {
			this->floorButtons[i].reading = digitalRead(this->floorButtons[i].pin);  //handle true
		}

		for(i=0;i<3;i++) {
				if (this->floorButtons[i].reading == false && this->floorButtons[i].lastButtonState == true) {
					this->machinist->handlePush(i + 1);
					Serial.printf("Btn %d pressed\n", i + 1);
				}
			this->floorButtons[i].lastButtonState = this->floorButtons[i].reading;
		}
		/*if ((millis() - this->floorButtons[i].lastDebounceTime) > this->debounceDelay) {
				this->floorButtons[i].lastDebounceTime = millis();
		}*/

		// A little delay to avoid many reads
		vTaskDelay(this->iterationDelay);
	}
}
