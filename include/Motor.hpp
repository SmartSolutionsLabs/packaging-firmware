#ifndef INC_MOTOR
#define INC_MOTOR

#include <Module.hpp>

class Motor : public Module {
	private:
		unsigned int stepPin;

		int steps;

		int delay;

		float stepDelay;

	public:
		Motor(const char * name, int taskCore = 1);

		void connect(void * data) override;

		void run(void* data) override;

		void off();

		void moveSteps(float speed, float length, float Kstepcm);

		void testSteps(int _steps);

		void setDelay(int delay);
};

#endif
