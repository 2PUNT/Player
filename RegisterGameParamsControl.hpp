#ifndef PLAYER_REGISTERGAMEPARAMSCONTROL_HPP
#define PLAYER_REGISTERGAMEPARAMSCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
#include "IKeyboardListener.hpp"

/// @file

/// \brief
/// Task for setting player values
/// \details
/// the function of this task is to set the playernumber, firepower, and time. After all these variables have been set this task starts the game.
/// The playernumber and firepower are set using an keypad. The playerNumber and firepower are set by pressing A or B Respectivly. Followed by a single digit (that is greater than 0).
/// The time and the signal to start the game are set by receiving an Message ADT through an rtos channel.
/// once the game has been started(a start signal has been received) this task wil be put in an eternal suspend.
class RegisterGameParamsControl: public rtos::task<>, public IKeyboardListener{
private:
	rtos::channel<char,10> KeyPressedRegQueue;
	rtos::channel<Message,10> MessagesReceivedRegQueue;
	enum class STATE {WAITING_ON_COMMAND_OR_USER_INPUT, WAITING_FOR_PLAYERNUMBER, WAITING_FOR_FIREPOWER};
	/// \brief
	/// Check if the PlayerID has been changed
	bool PlayerIDCheck = 0;
	/// \brief
	/// Check if the FirePower has been changed
	bool FirePowerCheck = 0;
	/// \brief
	/// Check if the GameTime has been changed
	bool TimeCheck = 0;
	
	void main();
public:
	RegisterGameParamsControl(const unsigned int priority, const char* name):
	task(priority, name), KeyPressedRegQueue(this, "KeyPressedRegQueue"), MessagesReceivedRegQueue(this, "MessagesReceivedRegQueue"){};
	
	void CommandReceived(Message Command);
	void KeyboardKeyPressed(char Key);
};

#endif