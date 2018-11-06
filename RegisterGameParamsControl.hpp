#ifndef PLAYER_REGISTERGAMEPARAMSCONTROL_HPP
#define PLAYER_REGISTERGAMEPARAMSCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
#include "IKeyboardListener.hpp"

class RegisterGameParamsControl: public rtos::task<>, public IKeyboardListener{
private:
	rtos::channel<char,10> KeyPressedRegQueue;
	rtos::channel<Message,10> MessagesReceivedRegQueue;
	enum class STATE {WAITING_ON_COMMAND_OR_USER_INPUT, WAITING_FOR_PLAYERNUMBER, WAITING_FOR_FIREPOWER};
	bool PlayerIDCheck = 0;
	bool FirePowerCheck = 0;
	bool TimeCheck = 0;
	
	void main();
public:
	RegisterGameParamsControl(const unsigned int priority, const char* name):
	task(priority, name), KeyPressedRegQueue(this, "KeyPressedRegQueue"), MessagesReceivedRegQueue(this, "MessagesReceivedRegQueue"){};
	
	void CommandReceived(Message Command);
	void KeyboardKeyPressed(char Key);
};

#endif