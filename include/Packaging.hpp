#ifndef _INC_PACKAGING_
#define _INC_PACKAGING_

#include <Application.hpp>
#include "Sensor.hpp"
#include "Motor.hpp"
#include "Machinist.hpp"
#include "Joypad.hpp"
#include "Display.hpp"

#define INDEX_MODULE_MACHINIST 0
#define INDEX_MODULE_JOYPAD 1
#define INDEX_MODULE_DISPLAY 2

#define CAST_MODULE_POINTER(type, index) (static_cast<type*>(this->modulesPointer[(index)]))

class Packaging : public Application {
	private:
		unsigned int speed;

	public:
		void processMessage(unsigned char * message, size_t length, bool printable);
		void initializeModulesPointerArray(unsigned int quantity) override;

		// Load all needed data
		void init();
};

#endif
