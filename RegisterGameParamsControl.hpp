#ifndef PLAYER_REGISTERGAMEPARAMSCONTROL_HPP
#define PLAYER_REGISTERGAMEPARAMSCONTROL_HPP

#include "rtos.hpp"

#include "IKeyboardListener.hpp"
// De Message ADT is niet beschreven in ADTs.hpp maar deze file gebruikt wel de Message ADT...
// De juiste file moet dus nog #include worden.

class RegisterGameParamsControl: public rtos::task<>, public IKeyboardListener{
public:
	void CommandReceived(Message Command);
	void KeyboardKeyPressed(char Key);
};

#endif
