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
	Serial.printf("Motor::run\n");
	GPIO.out_w1ts = (1 << this->stepPin);
	bool flag_steps_acel = true;
	this->suspend();
	static int i;

	// this loop must not die
	while (1) {
		
		this->working = true;
		float calc_delay = 1000 * this->stepDelay / 2;
		int interDelay = (int)calc_delay;

		int secure_time_to_acelerate = 400;
		int minDelay = interDelay; // Guardamos el mínimo
        int maxDelay = secure_time_to_acelerate; // Delay inicial
		int counter_secure_steps_to_acelerate = 0;
		int Delay = maxDelay;
		int steps_at_max_speed = this->steps ; // La mitad del recorrido para acelerar
        if(minDelay < 12) minDelay = 12;
		i = this->steps;
		int spendtime = millis();

		// the real work
		while(i>0) {
			i--;
			GPIO.out_w1tc = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de subida
			ets_delay_us(Delay);
			GPIO.out_w1ts = (1 << this->stepPin);
			//vTaskDelay(this->iterationDelay); //pulso de bajada
			ets_delay_us(Delay);

			// accelerate
			if (i < this->steps && Delay > minDelay && flag_steps_acel) {
                Delay -= 1;
				counter_secure_steps_to_acelerate ++; 
                if (Delay < minDelay) {
					Delay = minDelay;
					flag_steps_acel = false;
				}
			}

            // Desaceleración: En la segunda mitad del recorrido
            if (i > this->steps - counter_secure_steps_to_acelerate && Delay <= maxDelay && flag_steps_acel == false) {
                Delay += 1;
                if (Delay > maxDelay) Delay = maxDelay;
            }
		}
		
		this->working = false;
		//Serial.printf(" ----------- Motor::finish\n");
		Serial.printf("spent time: %d \n ", millis()-spendtime);

		Serial.printf("Motor finished moving, calling suspend()\n");

		Serial.printf(" * interDelay : %d \n", interDelay);

		vTaskDelay(1);

		Serial.printf("steps: %d \n" , this->steps);

		Serial.printf("LENGTH : %f \n", this->length);

		Serial.printf("stepDelay : %f \n", this->stepDelay);

		
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
	this->length = length;

	                // 8000 step/rev  == 100 steps / mm 
	Kstepcm = 62.5; // 5000 step/rev  == 62.5 steps / mm

	float time; //time , en milisegundos
	if (speed !=0){
		time = 1000 * length / speed; // 1000 ms
	}
	else {
		time = 0 ;
		return;
	}

	this->steps = (int) (length * Kstepcm); // mm * steps/mm 

	this->stepDelay = time / (2 * steps); // 1000 / (2*6350) =  0.07874 ms = 78 us
	
	this->working = true ;
	
	Serial.printf("time : %f \n", time);

	this->resume();
}

void Motor::testSteps(int _steps){
	
	this->steps = 200 ; // mm * steps/mms
	this->stepDelay = 0.20; // 1000 / (2*6350) =  0.07874 ms = 78 us

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