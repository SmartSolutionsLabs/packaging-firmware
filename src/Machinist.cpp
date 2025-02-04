#include "Machinist.hpp"

Machinist::Machinist(const char * name, int taskCore) : Module(name, taskCore), screen(SPEED) {
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
}
