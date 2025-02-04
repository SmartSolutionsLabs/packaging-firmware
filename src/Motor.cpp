#include "Motor.hpp"

Motor::Motor(const char * name, int taskCore) : Module(name, taskCore) {
}

void Motor::connect(void * data) {
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(14, OUTPUT);
	digitalWrite(12, HIGH);
	digitalWrite(13, HIGH);
	digitalWrite(14, HIGH);
}

void Motor::run(void* data) {
	while (1) {
		vTaskDelay(this->iterationDelay);
	}
}

void Motor::off() {
	delay(150);
	digitalWrite(13, HIGH);
	digitalWrite(14, HIGH);
	Serial.println("Motor::off");
}

void Motor::up() {
	digitalWrite(13, LOW);
	digitalWrite(14, HIGH);
	Serial.println("Motor::up");
}

void Motor::down() {
	digitalWrite(13, HIGH);
	digitalWrite(14, LOW);
	Serial.println("Motor::down");
}
