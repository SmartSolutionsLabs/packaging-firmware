#ifndef INC_JOYPAD
#define INC_JOYPAD

#include <Module.hpp>

#include "Machinist.hpp"

class Joypad : public Module {
	private:
		struct Button {
			uint8_t pin;
			int lastButtonState = HIGH;
			int buttonState = HIGH;
			int reading;
			unsigned long lastDebounceTime = 0;
		};

		const unsigned long debounceDelay = 20; // debounce time in milliseconds

		// Buttons for calling the floor where is pressed
		Button floorButtons[3];

		Machinist * machinist;

	public:
		Joypad(const char * name, int taskCore = 1);

		void connect(void * data) override;

		void run(void* data) override;

		void setMachinist(Machinist * newMachinist) {
			machinist = newMachinist;
		}
};

#endif
