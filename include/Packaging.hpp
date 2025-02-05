#ifndef _INC_PACKAGING_
#define _INC_PACKAGING_

#include <Application.hpp>
#include "Sensor.hpp"
#include "Motor.hpp"
#include "Machinist.hpp"
#include "Joypad.hpp"
#include "Display.hpp"

// About Bluetooth
#define BLE_SERVICE_UUID  "49e0b347-e722-4ac0-92fb-a316e887fdea"
#define BLE_PINGPONG_UUID "a1e00971-7498-4cd5-9cd6-b03c1d394770"  // Read and write
#define BLE_STATUS_UUID   "a12f4c9e-503b-45eb-8d7b-7fb774cf51d0"  // Notify sensor data

#define INDEX_MODULE_MACHINIST 0
#define INDEX_MODULE_JOYPAD 1
#define INDEX_MODULE_DISPLAY 2
#define INDEX_MODULE_SENSOR 3
#define INDEX_MODULE_MOTOR 4

#define CAST_MODULE_POINTER(type, index) (static_cast<type*>(this->modulesPointer[(index)]))

class Packaging : public Application {
	public:
		void processMessage(unsigned char * message, size_t length, bool printable);
		void initializeModulesPointerArray(unsigned int quantity) override;
		void initializeBluetoothCharacteristicsArray();
};

#endif
