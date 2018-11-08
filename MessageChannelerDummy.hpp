#ifndef _MESSAGECHANNELERDUM
#define _MESSAGECHANNELERDUM

#include "RegisterGameParamsControl.hpp"
#include "Entities.hpp"
#include "ADTS.hpp"
#include "IKeyboardListener.hpp"
#include "Keyboard_4x4.hpp"

class MessageChannelerDummy: public rtos::task<>{
	RegisterGameParamsControl& RegisterControl;
	ProcessHitControl& hitcontrol;
	void main();
public:
	MessageChannelerDummy(const unsigned int priority, const char* taskName, RegisterGameParamsControl& RegisterControl, ProcessHitControl& hitcontrol):
	task(priority, taskName), RegisterControl(RegisterControl), hitcontrol(hitcontrol){}
};

#endif