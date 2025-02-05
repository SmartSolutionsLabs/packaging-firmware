#include "Motor.hpp"

Motor::Motor(const char * name, int taskCore) : Module(name, taskCore) {
}

void Motor::connect(void * data) {
	this->stepPin = * (unsigned int *) data;

	pinMode(this->stepPin, OUTPUT);
	digitalWrite(this->stepPin, LOW);
}

void Motor::run(void* data) {
	while (1) {
		vTaskDelay(this->iterationDelay);
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

	int steps = length / Kstepcm;
	TickType_t xDelay = (time / 2 * steps) / portTICK_PERIOD_MS;

	for (int i=0; i < steps; i++){
		digitalWrite(this->stepPin, HIGH);
		vTaskDelay(xDelay); //pulso de subida
		digitalWrite(this->stepPin, LOW);
		vTaskDelay(xDelay); //pulso de bajada
	}

	Serial.println("Motor::Stepping");
}
