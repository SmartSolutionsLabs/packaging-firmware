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
	static int i;

	// this loop must not die
	while (1) {
		
		this->working = true;
		float calc_delay = 1000 * this->stepDelay / 3.95;
		int interDelay = (int)calc_delay;
		int delay = 100;
		Serial.printf(" * interDelay : %d \n", interDelay);
		i = this->steps;
		int spendtime = millis();

		// the real work
		while(i>0) {
			i--;

			GPIO.out_w1tc = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de subida
			ets_delay_us(delay);
			GPIO.out_w1ts = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de bajada
			ets_delay_us(delay);

			// accelerate
			if (delay > interDelay) {
				delay -= 1; //aumentar delay desacelera
				if (delay < interDelay) {
					delay = interDelay;
				}
			}
		}
		
		this->working = false;
		//Serial.printf(" ----------- Motor::finish\n");
		Serial.println("Motor finished moving, calling suspend()");
		vTaskDelay(1);
		this->suspend();
	}
}

bool Motor::isWorking(){
	return this->working;
}

void Motor::off() {
	digitalWrite(this->stepPin, HIGH);
	Serial.println("Motor::off");
}

void Motor::moveSteps(float speed, float length, float Kstepcm) { //length en cm , speed en cm/s ,Kstepcm en cm/tick
	Serial.printf("Motor::moveSteps called -> Speed: %.2f, Length: %.2f, Kstepcm: %.2f\n", speed, length, Kstepcm);
	//length = 63.5; //mm
	//kstepcm = mm/paso ; set 2000 : 0.039607 mm/paso; 25.24800000 pasos/mm
	// set 8000 :  101 pasos / mm
	//speed = 63.5; // mm/s
    Kstepcm = 62.5;
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

	/*Serial.printf("steps: %d \n" , this->steps);

	Serial.printf("LENGTH : %f \n", length);

	Serial.printf("stepDelay : %f \n", this->stepDelay);

	Serial.printf("time : %f \n", time);
	*/
	this->working = true ;
	this->resume();
}

void Motor::testSteps(int _steps){
	
	this->steps = 1000 ; // mm * steps/mms
	this->stepDelay = 0.05; // 1000 / (2*6350) =  0.07874 ms = 78 us

	this->resume();
}

/*void Motor::aceleration(int pasos){
	int delay_actual = step_delay * 2; //empieza lento
	for{int i=0; i< pasos; i++){
		digitalWrite(this->stepPin, HIGH);
		delayMicroseconds(delay_actual);
		digitalWrite(this->stepPin, LOW);
		delayMicroseconds(delay_actual);
		if(delay_actual > min_delay){
			delay_actual -= (delay_actual - min_delay) / pasos;
		}
	}
}*/