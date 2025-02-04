#include "Machinist.hpp"

Machinist::Machinist(const char * name, int taskCore) : Module(name, taskCore), state(READY) {
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
	this->floorStates[floorIndex - 1] = value;
	this->calculateFloor();
	this->work();
}

void Machinist::handleTargetFloor(unsigned int targetFloor) {
	if (targetFloor == 0 || targetFloor > this->upperLimit) {
		Serial.print("target floor out of bound\n");
		return;
	}

	this->targetFloor = targetFloor;
	Serial.printf("target floor %d \n", targetFloor);
	if(this->countdownHandTimer == nullptr) {
		static const esp_timer_create_args_t countdown_timer_args = {
			.callback = [](void* arg) {
				auto* lambda = static_cast<std::function<void()>*>(arg);
				(*lambda)(); // Execute lambda
			},
			.arg = &this->privateAction,
			.dispatch_method = ESP_TIMER_TASK,
			.name = "machinist-countdown"
		};
		esp_timer_create(&countdown_timer_args, &this->countdownHandTimer);
		esp_timer_start_once(this->countdownHandTimer, 5000000); // One shot after 5 seconds
	}
}

void Machinist::connect(void * data) {
}

void Machinist::run(void* data) {
	while (1) {
		vTaskDelay(3 * this->iterationDelay);
	}
}

void Machinist::work(){
	Serial.println("I will work...\n");
	if(this->state == GOING_DOWN || this->state == GOING_UP ){
		if(!this->calculateFloor()){
			Serial.println("between floors");
			Serial.printf("this->currentFloor = %d \n",this->currentFloor);
			return;
		}
		else {
			// Error because I can't decide
			Serial.println("Just Passing Floor");
		}
	}

	// Turn off because we arrived
	if(this->currentFloor == this->targetFloor) {
		this->motor->off();
		if(this->state == HOME){
			this->setState(READY);
		}
		// I hope everything is right
		else if(this->state == GOING_DOWN || this->state == GOING_UP){
			this->setState(ARRIVED);
		}
		else if(this->state == ARRIVED){
			this->setState(WAITING);
		}
		Serial.printf("Will not move from %d \n", (this->currentFloor));
		return;
	}

	if(this->currentFloor < this->targetFloor) {
		this->motor->up();
		this->setState(GOING_UP);
		Serial.printf("Will move from %d to %d \n", (this->currentFloor), (this->targetFloor));
		return;
	}

	if(this->currentFloor > this->targetFloor) {
		this->motor->down();
		this->setState(GOING_DOWN);
		Serial.printf("Will move from %d to %d \n", (this->currentFloor), (this->targetFloor));
		return;
	}

}

void Machinist::setFloorStates(bool floor1 , bool floor2, bool floor3){
	this->floorStates[0] = floor1;
	this->floorStates[1] = floor2;
	this->floorStates[2] = floor3;

	if(!this->calculateFloor()){
		Serial.println("error calculating floor Setting 3 .....");
		this->currentFloor = 3;
	}
	this->handleTargetFloor(1);
	work();
}

void Machinist::setState(Work newState){
	this->state = newState;
	Serial.println("\t ---- State of Machinist :  " + this->dictionary[this->state] + " ---");
}

bool Machinist::calculateFloor(){
	for(int i=0 ; i<3 ;i++){
		Serial.print("\t|\t");
		Serial.print(this->floorStates[0]);
	}
	Serial.println("\t|\t");

	if (this->floorStates[0] == true && this->floorStates[1] == false && this->floorStates[2] == false) {
		this->currentFloor = 1;
		Serial.println("\t ==== floor set 1 ====");
		return true;

	}
	else if (this->floorStates[1] == true && this->floorStates[0] == false && this->floorStates[2] == false) {
		this->currentFloor = 2;
		Serial.println("\t ==== floor set 2 ====");
		return true;
	}
	else if (this->floorStates[2] == true && this->floorStates[1] == false && this->floorStates[1] == false){
		this->currentFloor = 3;
		Serial.println("\t ==== floor set 3 ====");
		return true;
	}
	else{
		return false;
	}
}