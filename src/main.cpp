#include "Packaging.hpp"

Packaging * packaging;

void setup() {
	packaging = new Packaging();
	packaging->beginSerialPort(Serial);
	packaging->initializeModulesPointerArray(6);
}

void loop() {
}
