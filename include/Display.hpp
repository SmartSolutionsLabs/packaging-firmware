#ifndef INC_DISPLAY
#define INC_DISPLAY

#include <Module.hpp>
#include <Screen.hpp>
#include <Wire.h>
#include <LCD_I2C.h>

class Display : public Module {
	private:
		LCD_I2C  * lcd;

	public:
		Display(const char * name, int taskCore = 1);

		void connect(void * data) override;

		void run(void* data) override;

		void write();

		void print(Screen screen,float speed, float delay);
};

#endif
