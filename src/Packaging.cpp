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

	this->modulesPointer[INDEX_MODULE_DISPLAY] = new Display("display");
	this->modulesPointer[INDEX_MODULE_DISPLAY]->connect(nullptr);

	this->modulesPointer[INDEX_MODULE_MACHINIST] = new Machinist("mac");
	this->modulesPointer[INDEX_MODULE_MACHINIST]->connect(nullptr);
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setDisplay(CAST_MODULE_POINTER(Display, INDEX_MODULE_DISPLAY));
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->showData();

	this->modulesPointer[INDEX_MODULE_JOYPAD] = new Joypad("jpd");
	this->modulesPointer[INDEX_MODULE_JOYPAD]->connect(nullptr);
	this->modulesPointer[INDEX_MODULE_JOYPAD]->start();

	this->init();
}

void Packaging::init() {
	Serial.println("Setting Home");
}
