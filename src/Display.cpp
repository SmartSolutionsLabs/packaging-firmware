#include "Display.hpp"

Display::Display(const char * name, int taskCore) : Module(name, taskCore) {

}

void Display::connect(void * data) {
	this->lcd = new LCD_I2C(0x27, 16, 2);
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
			this->lcd->print("Speed :");
			this->lcd->setCursor(8,0);
			this->lcd->print(speed);

			this->lcd->setCursor(0,1);
			this->lcd->print("Delay :");
			this->lcd->setCursor(8,0);
			this->lcd->print(delay);
			break;
		case SPEED:
			this->lcd->setCursor(0,0);
			this->lcd->print("      Speed     ");
			this->lcd->setCursor(3,1);
			this->lcd->print(speed);
			this->lcd->setCursor(9,1);
			this->lcd->print(" cm/s");
			break;
		case DELAY:
			this->lcd->setCursor(0,0);
			this->lcd->print("Delay");
			this->lcd->setCursor(0,1);
			this->lcd->print(delay);
			this->lcd->print(" ms");
			break;
	}
}
