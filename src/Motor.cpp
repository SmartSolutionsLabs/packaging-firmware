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

	
	while (1) {
		ets_delay_us(this->delay*1000); // microseconds
		Serial.printf("Motor::Stepping\n");

		float calc_delay = 1000 * this->stepDelay / 3.95; 
		int interDelay = (int)calc_delay;
	    Serial.printf(" ------------ interDelay : %d \n", interDelay);
		int i = this->steps;
		int spendtime = millis();
		while(i>0) {
			i--;
			GPIO.out_w1tc = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de subida
			ets_delay_us(interDelay);
			GPIO.out_w1ts = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de bajada
			ets_delay_us(interDelay);
		}
		Serial.printf(" ----------- Motor::finish\n");
		Serial.printf("spent time = %d \n", millis()- spendtime);
		
		this->suspend();
	}
}

void Motor::off() {
	digitalWrite(this->stepPin, HIGH);
	Serial.println("Motor::off");
}

void Motor::moveSteps(float speed, float length, float Kstepcm) { //length en cm , speed en cm/s ,Kstepcm en cm/tick
    //length = 63.5; //mm
	//kstepcm = mm/paso ; set 2000 : 0.039607 mm/paso; 25.24800000 pasos/mm
	// set 8000 :  101 pasos / mm
	//speed = 63.5; // mm/s

	float time; //time , en milisegundos
	if (speed !=0){
		time = 1000 * length / speed; // 1000 ms
	}
	else {
		time = 0 ;
		return;
	}

	this->steps = length * Kstepcm; // mm * steps/mm

	this->stepDelay = time / (2 * steps); // 1000 / (2*6350) =  0.07874 ms = 78 us

	Serial.printf("steps: %d \n" , this->steps);
		
	Serial.printf("LENGTH : %f \n", length);
		
	Serial.printf("stepDelay : %f \n", this->stepDelay);
		
	Serial.printf("time : %f \n", time);
	
	this->resume(); 
}

void Motor::testSteps(int _steps){
	int speed = 63.5;
	float time; //time , en milisegundos
	float length = _steps/100;
	if (speed !=0){
		time = 1000 *  length / speed; // 1000 ms
	}
	else {
		time = 0 ;
		return;
	}

	this->steps = _steps ; // mm * steps/mm

	this->stepDelay = time / (2 * steps); // 1000 / (2*6350) =  0.07874 ms = 78 us

	this->resume();
}

void Motor::setDelay(int delay) {
	this->delay = delay;
}
