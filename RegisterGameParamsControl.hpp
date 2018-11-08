#ifndef PLAYER_REGISTERGAMEPARAMSCONTROL_HPP
#define PLAYER_REGISTERGAMEPARAMSCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
#include "IKeyboardListener.hpp"
#include "Entities.hpp"
#include "ProcessHitControl.hpp"

/// @file

/// @class RegisterGameParamsControl
/// @brief
/// Task for setting player values
/// @details
/// the function of this task is to set the playernumber, firepower, and time. After all these variables have been set this task starts the game.
/// The playernumber and firepower are set using an keypad. The playerNumber and firepower are set by pressing A or B Respectivly. Followed by a single digit (that is greater than 0).
/// The time and the signal to start the game are set by receiving an Message ADT through an rtos channel.
/// once the game has been started(a start signal has been received) this task wil be put in an eternal suspend.
class RegisterGameParamsControl: public rtos::task<>, public IKeyboardListener{
private:
	rtos::timer startTimer;
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
	ProcessHitControl& processHitControl;
	RemainingTime& remainingTime;
	
	//UpdateGameTimeControl& gameTimeControl;
	//ShootControl& shootControl;
	PlayerData& playerData;
	//IRunGameTask& RunGame;
	
	void main();
public:
	///@fn RegisterGameParamsControl::RegisterGameParamsControl(const unsigned int priority, const char* name)
	///@brief The constructor for the RegisterGameParamsControl class.
	///details This constructor creates a RegisterGameParamsControl object.
	///@param priority Priority of the task.
	///@param name Name of the task.
	RegisterGameParamsControl(const unsigned int priority, const char* name, RemainingTime& _remainingTime, ProcessHitControl& _processHitControl, /*UpdateGameTimeControl& _gameTimeControl, ShootControl& _shootControl, */PlayerData& _playerData):
		task(priority, name), startTimer(this, "startTimer"), KeyPressedRegQueue(this, "KeyPressedRegQueue"),MessagesReceivedRegQueue(this, "MessagesReceivedRegQueue"), /*RunGame(RunGame),MessagesReceivedRegQueue(this, "MessagesReceivedRegQueue"),
		remainingTime(_remainingTime), gameTimeControl(_gameTimeControl), shootControl(_shootControl),
		*/ processHitControl(_processHitControl), remainingTime(_remainingTime),playerData(_playerData){};
	
	///@fn void RegisterGameParamsControl::CommandReceived(Message Command)
	///@brief Receive command Message.
	///@details This function writes the command message to a pool.
	///@param The Command to write.
	void CommandReceived(Message Command);
	
	///@fn void RegisterGameParamsControl::KeyboardKeyPressed(char Key)
	///@brief Receive Key
	///@details This function writes the Key to a pool.
	///@param Key The Key to write.
	void KeyboardKeyPressed(char Key);
};

#endif