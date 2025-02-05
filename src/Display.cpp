#include "Display.hpp"

Display::Display(const char * name, int taskCore) : Module(name, taskCore) {

}

void Display::connect(void * data) {
	this->lcd = new LCD_I2C(0x27, 16, 2);

	static byte label_missing[] = {
								B00101,
								B00010,
								B00101,
								B10000,
								B10000,
								B10000,
								B10000,
								B11100
								};
	static byte label_found[] = {
								B00000,
								B00000,
								B00000,
								B10000,
								B10000,
								B10000,
								B10000,
								B11100
								};
	static byte bottle_missing[] = {
								B00101,
								B00010,
								B00101,
								B10000,
								B10000,
								B11100,
								B10100,
								B11100
								};
	static byte bottle_found[] = {
								B00000,
								B00000,
								B00000,
								B10000,
								B10000,
								B11100,
								B10100,
								B11100
								};


	this->lcd->createChar(0, bottle_missing);
	this->lcd->createChar(1, bottle_found);
	this->lcd->createChar(2, label_missing);
	this->lcd->createChar(3, label_found);

	this->lcd->begin();

	this->lcd->backlight();
	this->lcd->setCursor(3, 0);
	this->lcd->print("Etiquetadora");
	Serial.print("Display connected.\n");
}

void Display::run(void* data) {
}

void Display::print(Screen screen, float speed, float delay){
	this->lcd->clear();

	switch (screen){
		case READY:
			this->lcd->setCursor(0,0);
			this->lcd->print("Speed : ");
			this->lcd->setCursor(8,0);
			this->lcd->print(speed,1);
			this->lcd->setCursor(12,0);
			this->lcd->print("cm/s");

			this->lcd->setCursor(0,1);
			this->lcd->print("Delay :");
			this->lcd->setCursor(8,1);
			this->lcd->print(delay,0);
			this->lcd->setCursor(12,1);
			this->lcd->print("ms");
			break;

		case SPEED:
			this->lcd->setCursor(0,0);
			this->lcd->print("      Speed     ");
			this->lcd->setCursor(4,1);
			this->lcd->print(speed,1);
			this->lcd->setCursor(9,1);
			this->lcd->print("cm/s");
			break;

		case DELAY:
			this->lcd->setCursor(0,0);
			this->lcd->print("      Delay     ");
			this->lcd->setCursor(4,1);
			this->lcd->print(delay,0);
			this->lcd->setCursor(9,1);
			this->lcd->print("ms");
			break;

		case CHANGE_SPEED:
			this->lcd->setCursor(0,0);
			this->lcd->print("      Speed     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("->");
			this->lcd->setCursor(4,1);
			this->lcd->print(speed,1);
			this->lcd->setCursor(10,1);
			this->lcd->print("cm/s");
			break;

		case CHANGE_DELAY:
			this->lcd->setCursor(0,0);
			this->lcd->print("      Delay     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("->");
			this->lcd->setCursor(4,1);
			this->lcd->print(delay,0);
			this->lcd->setCursor(9,1);
			this->lcd->print("ms");
			break;

		case LABEL_MISSING:
			this->lcd->setCursor(0,0);
			this->lcd->print("     LABEL      ");
			this->lcd->setCursor(0,1);
			this->lcd->print("     MISSING    ");
			break;
		
		case LABEL_FOUND:
			this->lcd->setCursor(0,0);
			this->lcd->print("     LABEL     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("     FOUND     ");
			break;
	}
}
