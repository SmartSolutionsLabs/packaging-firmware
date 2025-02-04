#ifndef INC_MACHINIST
#define INC_MACHINIST

#include <Module.hpp>

#include <Preferences.h>

#include "Motor.hpp"

// Used to hold current screen of menu
enum Screen {
	SPEED
};

class Machinist : public Module {
	private:
		Screen screen;

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

		Screen getScreen() {
			return screen;
		}

		void setMotor(Motor * newMotor) {
			motor = newMotor;
		}

		void setScreen(Screen newScreen) {
			screen = newScreen;
		}
};

#endif
