#ifndef INC_MACHINIST
#define INC_MACHINIST

#include <Module.hpp>

#include <Preferences.h>

#include "Motor.hpp"
#include "Screen.hpp"
#include "Display.hpp"

class Machinist : public Module {
	private:
		Screen screen;

		Motor * motor;

		bool enabled = false;

		Display * display;

		Preferences * preferences;

		float speed;

		float delay;

		float labelLength;

		float speedStep;

		float delayStep;

		// For manual movement
		int testStep;

		float Kstepcm;
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

		// Flag used in thread loop
		bool newBottle = false;

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
		 * We have three so we can use -1 as DOWN, 0 as ENTER, +1 as UP.
		 */
		void handlePush(int key);

		Screen getScreen() {
			return screen;
		}

		void setMotor(Motor * newMotor) {
			motor = newMotor;
		}

		void setScreen(Screen newScreen) {
			screen = newScreen;
		}

		void showData();

		void setDisplay(Display * newDisplay) {
			display = newDisplay;
		}

		// Just setting within bounds
		void setSpeed(float speed);
		void setDelay(float delay);
		void setLabelLength(float length);

		// Save into ROM new "constants"
		void saveSpeed();
		void saveDelay();
		void saveSpeedStep(float step);
		void saveDelayStep(float step);
		void saveLabelLength();
		void saveKstepcm(float newKstepcm);
		void saveTestStep(int step);

		void test(int _steps);

		void enable(bool enabled);
};

#endif
