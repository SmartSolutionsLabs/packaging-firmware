#include "Elevator.hpp"

Elevator * elevator;

void setup() {
	elevator = new Elevator();
	elevator->beginSerialPort(Serial);
	elevator->initializeModulesPointerArray(6);
}

void loop() {
}
