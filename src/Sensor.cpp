#include "Sensor.hpp"

Sensor::Sensor(const char * name, int taskCore) : Module(name, taskCore) {
}

void Sensor::connect(void * data) {
	this->pin = * (uint8_t *) data;
	pinMode(this->pin , INPUT);
	this->lastReading = digitalRead(this->pin);
	this->lastDebounceTime = millis();
	this->debugTimer = 1000;
}

void Sensor::run(void* data) {
	this->iterationDelay = 10 / portTICK_PERIOD_MS;
	vTaskDelay(this->iterationDelay);
	while (1) {
		this->reading = digitalRead(this->pin); // LOW is pressed
		/*if (millis() - this->debugTimer > 1000 ){
			this->debugTimer = millis();
			Serial.print(this->name);
			Serial.print("\t value: ");
			Serial.println(this->buttonState);
		}*/
		if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
			this->lastDebounceTime = millis();

			if (this->reading == false && this->lastReading == true){
				this->lastButtonState = this->buttonState;
				this->buttonState = true;
				this->lastReading = this->reading;
				this->machinist->handleArrivedFloor(this->floorIndex, this->buttonState);
				//Serial.printf("Sensor %d rise\n", this->floorIndex);
			}
			else if (this->reading == true && this->lastReading == false){
				this->lastButtonState = this->buttonState;
				this->buttonState = false;
				this->lastReading = this->reading;
				this->machinist->handleArrivedFloor(this->floorIndex, this->buttonState);
				//Serial.printf("Sensor %d fall\n", this->floorIndex);
			}
		}
		this->lastButtonState = this->reading;

		// A little delay to avoid many reads
		//
	}
}

bool Sensor::getValue(){
	return this->buttonState;
}

bool Sensor::getStartValue(){
	return !digitalRead(this->pin);
}