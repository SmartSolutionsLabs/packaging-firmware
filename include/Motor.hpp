#ifndef INC_MOTOR
#define INC_MOTOR

#include <Module.hpp>

class Motor : public Module {
	private:
		unsigned int stepPin;

	public:
		Motor(const char * name, int taskCore = 1);

		void connect(void * data) override;

		void run(void* data) override;

		void off();

		void moveSteps(float speed, float length, int Kstepcm);
};

#endif
