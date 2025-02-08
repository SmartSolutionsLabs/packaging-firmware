#include "Packaging.hpp"

#include <BluetoothLowEnergy.hpp>

void Packaging::processMessage(unsigned char * message, size_t length, bool printable) {
	// String class to use easy methods
	String msgStr = String((char*)message);

	// No longer needed
	delete[] message;

	// Look for separator ":"
	int separatorIndex = msgStr.indexOf(':');
	if (separatorIndex == -1) {
		Serial.print("Format option:value.\n");
		#ifdef __SMART_APPLICATION_WITH_BLE__
		BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "Format option:value.\n");
		#endif
		return;
	}

	// before :
	int option = msgStr.substring(0, separatorIndex).toInt();

	// after :
	float value = msgStr.substring(separatorIndex + 1).toFloat();

	switch (option) {
		case 1: // setting speed step
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveSpeedStep(value);
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new SPEED step\n");
			#endif
			break;
		case 2: // setting delay step
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveDelayStep(value);
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new DELAY step\n");
			#endif
			break;
		case 3: // setting LabelLength
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setLabelLength(value);
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveLabelLength();
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new LabelLength \n");
			#endif
			break;
		case 4: // setting Kstepcm
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveKstepcm(value);
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new Kstepcm \n");
			#endif
			break;
		case 5: // force setting delay
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setDelay(value);
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveDelay();
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new initial Delay \n");
			#endif
			break;
		case 6: // setting test step (manual step value)
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->saveTestStep((int) value);
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "new test steps \n");
			#endif
			break;
		case 10: // testing mode with steps
			CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->test((int)value);
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "Testing \n");
			#endif
			break;

		case 100:
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "Help: \n 1: Set SpeedStep \n 2: Set delayStep \n 3: Set LabelLength \n 4: Set Steps/mm \n 5: Set InitialDelay \n 6: Set Test Steps \n");
			#endif
			break;

		default:
			#ifdef __SMART_APPLICATION_WITH_BLE__
			BluetoothLowEnergy::sendOut(&this->bleCharacteristics[0], "option:value (option 1 is speed; option 2 is delay)\n");
			#endif
			break;
	}
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

	// Pin number for sensor
	uint8_t pin;

	this->modulesPointer[INDEX_MODULE_DISPLAY] = new Display("display", 0);
	this->modulesPointer[INDEX_MODULE_DISPLAY]->connect(nullptr);

	pin = 18;
	this->modulesPointer[INDEX_MODULE_MOTOR] = new Motor("mtr", 1); // using second core
	this->modulesPointer[INDEX_MODULE_MOTOR]->connect(&pin);
	this->modulesPointer[INDEX_MODULE_MOTOR]->start();

	this->modulesPointer[INDEX_MODULE_MACHINIST] = new Machinist("mac", 0);
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setMotor(CAST_MODULE_POINTER(Motor, INDEX_MODULE_MOTOR));
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->setDisplay(CAST_MODULE_POINTER(Display, INDEX_MODULE_DISPLAY));
	this->modulesPointer[INDEX_MODULE_MACHINIST]->connect(nullptr);
	this->modulesPointer[INDEX_MODULE_MACHINIST]->start();
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->showData();

	this->modulesPointer[INDEX_MODULE_JOYPAD] = new Joypad("jpd", 0);
	this->modulesPointer[INDEX_MODULE_JOYPAD]->connect(nullptr);
	this->modulesPointer[INDEX_MODULE_JOYPAD]->start();

	pin = 17;
	this->modulesPointer[INDEX_MODULE_SENSOR_BOTTLE] = new Sensor("snrBtl", 0);
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_BOTTLE)->setFloor(0);
	this->modulesPointer[INDEX_MODULE_SENSOR_BOTTLE]->connect(&pin);
	this->modulesPointer[INDEX_MODULE_SENSOR_BOTTLE]->start();

	pin = 5;
	this->modulesPointer[INDEX_MODULE_SENSOR_LABEL] = new Sensor("snrLbl", 0);
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_LABEL)->setFloor(1);
	this->modulesPointer[INDEX_MODULE_SENSOR_LABEL]->connect(&pin);
	this->modulesPointer[INDEX_MODULE_SENSOR_LABEL]->start();

	// Passing references with machinist
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_BOTTLE)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));
	CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_LABEL)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));
	CAST_MODULE_POINTER(Joypad, INDEX_MODULE_JOYPAD)->setMachinist(CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST));

	// Read label sensor value to enable mechanism
	CAST_MODULE_POINTER(Machinist, INDEX_MODULE_MACHINIST)->enable(
		CAST_MODULE_POINTER(Sensor, INDEX_MODULE_SENSOR_LABEL)->getStartValue()
	);
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
