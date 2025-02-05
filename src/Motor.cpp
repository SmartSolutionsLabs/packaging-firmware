#include "Motor.hpp"

Motor::Motor(const char * name, int taskCore) : Module(name, taskCore) {
}

void Motor::connect(void * data) {
	this->stepPin = * (unsigned int *) data;

	pinMode(this->stepPin, OUTPUT);
	digitalWrite(this->stepPin, LOW);
}

void Motor::run(void* data) {
	// Just start this thread, but do nothing
	Serial.print("Motor::run\n");
	this->suspend();

	while (1) {
		vTaskDelay(this->iterationDelay);

		Serial.print("Motor::Stepping\n");

		for (int i = 0; i < this->steps; ++i) {
			digitalWrite(this->stepPin, HIGH);
			vTaskDelay(this->iterationDelay); //pulso de subida
			digitalWrite(this->stepPin, LOW);
			vTaskDelay(this->iterationDelay); //pulso de bajada
		}

		this->suspend();
	}
}

void Motor::off() {
	digitalWrite(this->stepPin, LOW);
	Serial.println("Motor::off");
}

void Motor::moveSteps(float speed, float length, int Kstepcm) { //length en cm , speed en cm/s ,Kstepcm en cm/tick
	int time; //time , en milisegundos
	if (speed !=0){
		time = 1000 * length / speed;
	}
	else {
		time = 0 ;
		return;
	}

	this->steps = length / Kstepcm;

	// It's used in thread loop
	this->iterationDelay = (time / 2 * this->steps) / portTICK_PERIOD_MS;

	this->resume();
}
