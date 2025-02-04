#ifndef INC_DISPLAY
#define INC_DISPLAY

#include <Module.hpp>

#include <LiquidCrystal.h>

#include "Machinist.hpp"

class Display : public Module {
	private:
		LiquidCrystal * lcd;

		Machinist * machinist;

	public:
		Display(const char * name, int taskCore = 1);

		void connect(void * data) override;

		void run(void* data) override;

		void setMachinist(Machinist * newMachinist) {
			machinist = newMachinist;
		}

		void write();

		void print(Screen screen,float speed, float delay);
};

#endif
