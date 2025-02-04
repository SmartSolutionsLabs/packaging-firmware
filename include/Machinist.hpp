#ifndef INC_MACHINIST
#define INC_MACHINIST

#include <Module.hpp>

#include "Motor.hpp"

enum Work {
	HOME, // initial work
	READY, // accepting instructions from anyone and executes "inmediately"
	GOING_DOWN, // ascending
	GOING_UP, // descending
	WAITING, // basket arrived from another floor to current floor
	ARRIVED, // work completed
	LOST // bewildered, disconcerted to take decisions
};

class Machinist : public Module {
	private:
		Work state;

		/**
		 * Where basket must go.
		 */

		String dictionary[7] = {"HOME","READY","GOING_DOWN","GOING_UP","WAITING","ARRIVED","LOST"};

		unsigned int currentFloor = 3;

		unsigned int targetFloor = 1;

		unsigned int upperLimit = 1;

		bool floorStates[3];

		Motor * motor;

		/**
		 * Performs movement of basket.
		 */
		void work();

		/**
		 * It's the countdown hand.
		 */
		esp_timer_handle_t countdownHandTimer = nullptr;

		/**
		 * Capsule to execute work.
		 */
		std::function<void()> privateAction;

	public:
		Machinist(const char * name, int taskCore = 1);

		~Machinist();

		void connect(void * data) override;

		void run(void* data) override;

		/**
		 * Modify values in floors array.
		 */
		void handleLeavedFloor(unsigned int floorIndex, bool value);

		void handleArrivedFloor(unsigned int floorIndex, bool value);

		/**
		 * Catch calling from buttons.
		 */
		void handleTargetFloor(unsigned int targetFloor);

		Work getState() {
			return state;
		}

		void setMotor(Motor * newMotor) {
			motor = newMotor;
		}

		void setUpperLimit(unsigned int limit) {
			upperLimit = limit;
		}

		unsigned int getUpperLimit() {
			return upperLimit;
		}

		void setFloorStates(bool floor1 , bool floor2, bool floor3);

		void setState(Work newState);

		bool calculateFloor();
};

#endif
