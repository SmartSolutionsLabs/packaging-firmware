#ifndef _INC_ELEVATOR_
#define _INC_ELEVATOR_

#include <Application.hpp>
#include "Sensor.hpp"
#include "Motor.hpp"
#include "Machinist.hpp"
#include "Joypad.hpp"

#define INDEX_MODULE_MOTOR 0

#define INDEX_MODULE_SENSOR_FLOOR_1 (INDEX_MODULE_MOTOR + 1)
#define INDEX_MODULE_SENSOR_FLOOR_2 (INDEX_MODULE_SENSOR_FLOOR_1 + 1)
#define INDEX_MODULE_SENSOR_FLOOR_3 (INDEX_MODULE_SENSOR_FLOOR_2 + 1)

#define INDEX_MODULE_JOYPAD (INDEX_MODULE_SENSOR_FLOOR_3 + 1)

#define INDEX_MODULE_MACHINIST (INDEX_MODULE_JOYPAD + 1)

#define CAST_MODULE_POINTER(type, index) (static_cast<type*>(this->modulesPointer[(index)]))

class Elevator : public Application {
	private:
		unsigned int speed;

	public:
		void processMessage(unsigned char * message, size_t length, bool printable);
		void initializeModulesPointerArray(unsigned int quantity) override;

		// Load all needed data
		void init();
};

#endif
