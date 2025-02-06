#include "Motor.hpp" // etiqueta de 160 mm , espaciado 3.5 mm = 163.5 mm == 327steps

Motor::Motor(const char * name, int taskCore) : Module(name, taskCore) {
}

void Motor::connect(void * data) {
	this->stepPin = * (unsigned int *) data;

	pinMode(this->stepPin, OUTPUT);
	digitalWrite(this->stepPin, HIGH);
}

void Motor::run(void* data) {
	// Just start this thread, but do nothing
	Serial.print("Motor::run\n");
	digitalWrite(this->stepPin, HIGH);
	this->suspend();

	static int interDelay = 250;

	while (1) {
		ets_delay_us(this->delay*1000); // microseconds
		Serial.printf("Motor::Stepping\n");
		Serial.printf("steps: %d \n" , this->steps);
		Serial.printf("iterationDelay : %d \n", (int)this->iterationDelay);
		
		for (int i = 0; i < this->steps; ++i) {
			digitalWrite(this->stepPin, LOW);
			//vTaskDelay(this->iterationDelay); //pulso de subida
			ets_delay_us(interDelay);
			digitalWrite(this->stepPin, HIGH);
			//vTaskDelay(this->iterationDelay); //pulso de bajada
			ets_delay_us(interDelay);
		}
		//Serial.print("Motor::finish\n");
		this->suspend();
	}
}

void Motor::off() {
	digitalWrite(this->stepPin, HIGH);
	Serial.println("Motor::off");
}

void Motor::moveSteps(float speed, float length, float Kstepcm) { //length en cm , speed en cm/s ,Kstepcm en cm/tick
    length = 163.5; //mm
	
	int time; //time , en milisegundos
	if (speed !=0){
		time = 1000 * (length / speed);
	}
	else {
		time = 0 ;
		return;
	}

	this->steps = length * Kstepcm; // mm * steps/mm

	// It's used in thread loop
	this->iterationDelay = (time / (this->steps * 2)) / portTICK_PERIOD_MS;

	this->resume();
}

void Motor::testSteps(int _steps){
	this->steps = _steps;

	// It's used in thread lo	op
	this->iterationDelay = 50 / portTICK_PERIOD_MS;

	this->resume();
}

void Motor::setDelay(int delay) {
	this->delay = delay;
}
