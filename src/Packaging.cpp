#include "Packaging.hpp"

void Packaging::processMessage(unsigned char * message, size_t length, bool printable) {
	unsigned int floor = (unsigned int) message[0] - '0';
	Serial.println(floor);
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->handleTargetFloor(floor);

	delete[] message;
}

void Packaging::initializeModulesPointerArray(unsigned int quantity) {
	if (this->modulesPointer != nullptr) {
		while (this->modulesPointerQuantity) {
			delete this->modulesPointer[--this->modulesPointerQuantity];
		}
		delete[] this->modulesPointer;
	}

	this->modulesPointerQuantity = quantity;

	this->modulesPointer = new Module*[quantity];

	this->modulesPointer[INDEX_MODULE_MOTOR] = new Motor("Motor");
	this->modulesPointer[INDEX_MODULE_MOTOR]->connect(nullptr);
	this->modulesPointer[INDEX_MODULE_MOTOR]->start();

	// Pin number for sensor
	uint8_t pin;

	pin = 16;
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_1] = new Sensor("SensorFloor1");
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_1]->connect(&pin);

	pin = 23;
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_2] = new Sensor("SensorFloor2");
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_2]->connect(&pin);

	pin = 19;
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_3] = new Sensor("SensorFloor3");
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_3]->connect(&pin);

	this->modulesPointer[INDEX_MODULE_JOYPAD] = new Joypad("jpd");
	this->modulesPointer[INDEX_MODULE_JOYPAD]->connect(nullptr);

	this->modulesPointer[INDEX_MODULE_MACHINIST] = new Machinist("machinist");
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setUpperLimit(3);

	// Passing machinist to all sensors
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_1)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_2)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_3)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));

	// Passing machinist to the joypad
	CAST_MODULE_POINTER(Joypad, INDEX_MODULE_JOYPAD)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));

	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_1)->setFloor(1);
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_2)->setFloor(2);
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_3)->setFloor(3);

	// starting sensors reading
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_1]->start();
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_2]->start();
	this->modulesPointer[INDEX_MODULE_SENSOR_FLOOR_3]->start();

	// starting buttons reading
	this->modulesPointer[INDEX_MODULE_JOYPAD]->start();

	this->init();
}

void Packaging::init() {
	Serial.println("Setting Home");
	bool initalStates[3] = {0,0,0};
	initalStates[0] = CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_1)->getStartValue();
	initalStates[1] = CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_2)->getStartValue();
	initalStates[2] = CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_FLOOR_3)->getStartValue();
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setFloorStates(initalStates[0],initalStates[1],initalStates[2]);
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setState(Work::HOME);

}
