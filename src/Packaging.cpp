#include "Packaging.hpp"

#include <BluetoothLowEnergy.hpp>

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
}

#ifdef __SMART_APPLICATION_WITH_BLE__
void Packaging::initializeBluetoothCharacteristicsArray() {
	if (this->bleCharacteristics != nullptr) {
		delete[] this->bleCharacteristics;
	}

	this->bluetoothCharacteristicsQuantity = 1;

	this->bleCharacteristics = new BLECharacteristic[1] {
		BLE_PINGPONG_UUID
	};

	this->bleCharacteristics[0].setWriteProperty(true);
	this->bleCharacteristics[0].setReadProperty(true);
	this->bleCharacteristics[0].setNotifyProperty(true);
	this->bleCharacteristics[0].addDescriptor(new BLEDescriptor(BLEUUID((uint16_t)0x2902)));
}

BluetoothLowEnergy * ble;
#endif

Packaging * packaging;

void setup() {
	packaging = new Packaging();
	packaging->beginSerialPort(Serial);
	packaging->initializeModulesPointerArray(6);

#ifdef __SMART_APPLICATION_WITH_BLE__
	packaging->setBluetoothName("Packaging BT", true);

	// We turn on safety
	ble = new BluetoothLowEnergy(packaging);
#endif
}

void loop() {
#ifdef __SMART_APPLICATION_WITH_BLE__
	ble->checkAdvertising();
#endif
}
