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

void Display::print(Screen screen, float speed, float delay, float length){
	this->lcd->clear();

	switch (screen){
		case READY:
			this->lcd->setCursor(1,0);
			this->lcd->print("S:");
			this->lcd->setCursor(3,0);
			this->lcd->print(speed,1);
			this->lcd->setCursor(0,1);
			this->lcd->print("<-            ->");
			this->lcd->setCursor(10,0);
			this->lcd->print("D:");
			this->lcd->setCursor(12,0);
			this->lcd->print(delay,0);
			break;

		case SPEED:
			this->lcd->setCursor(0,0);
			this->lcd->print("      SPEED     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("<-  ");
			this->lcd->setCursor(14,1);
			this->lcd->print("->");
			this->lcd->setCursor(4,1);
			this->lcd->print(speed,1);
			this->lcd->setCursor(9,1);
			this->lcd->print("mm/s");
			break;

		case DELAY:
			this->lcd->setCursor(0,0);
			this->lcd->print("      DELAY     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("<-");
			this->lcd->setCursor(14,1);
			this->lcd->print("->");
			this->lcd->setCursor(4,1);
			this->lcd->print(delay,0);
			this->lcd->setCursor(9,1);
			this->lcd->print("ms");
			break;

		case LENGTH:
			this->lcd->setCursor(0,0);
			this->lcd->print("     LENGTH     ");
			this->lcd->setCursor(0,1);
			this->lcd->print("<-");
			this->lcd->setCursor(14,1);
			this->lcd->print("->");
			this->lcd->setCursor(4,1);
			this->lcd->print(length,3);
			this->lcd->setCursor(10,1);
			this->lcd->print("mm");
			break;

		case CHANGE_SPEED:
			this->lcd->setCursor(0,0);
			this->lcd->print("    NEW SPEED    ");
			this->lcd->setCursor(1,1);
			this->lcd->print("-");
			this->lcd->setCursor(14,1);
			this->lcd->print("+");
			this->lcd->setCursor(4,1);
			this->lcd->print(speed,1);
			this->lcd->setCursor(9,1);
			this->lcd->print("mm/s");
			break;

		case CHANGE_DELAY:
			this->lcd->setCursor(0,0);
			this->lcd->print("   NEW  DELAY   ");
			this->lcd->setCursor(1,1);
			this->lcd->print("-");
			this->lcd->setCursor(14,1);
			this->lcd->print("+");
			this->lcd->setCursor(4,1);
			this->lcd->print(delay,0);
			this->lcd->setCursor(10,1);
			this->lcd->print("ms");
			break;

		case CHANGE_LENGTH:
			this->lcd->setCursor(0,0);
			this->lcd->print("   NEW LENGTH   ");
			this->lcd->setCursor(1,1);
			this->lcd->print("-");
			this->lcd->setCursor(14,1);
			this->lcd->print("+");
			this->lcd->setCursor(4,1);
			this->lcd->print(length,3);
			this->lcd->setCursor(11,1);
			this->lcd->print("mm");
			break;

		case OFFSET:
			this->lcd->setCursor(0,0);
			this->lcd->print("  MANUAL MOVE   ");
			this->lcd->setCursor(0,1);
			this->lcd->print("<-    Enter   ->");
			break;
		
		case MANUAL_MOVEMENT:
			this->lcd->setCursor(0,0);
			this->lcd->print("  MANUAL MOVE   ");
			this->lcd->setCursor(0,1);
			this->lcd->print("      Back  Move");
			break;
	}
}
