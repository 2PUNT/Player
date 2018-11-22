#ifndef PLAYER_UPDATEGAMETIMECONTROL_HPP
#define PLAYER_UPDATEGAMETIMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "DigitLedDisplay.hpp"
#include "Entities.hpp"
#include "ShootControl.hpp"
#include "ProcessHitControl.hpp"
#include "TransferData.hpp"

/// @file

/// \brief
/// Task for keeping the remaining game time
/// \details
/// This Task keeps track of the remaining game time and updates it in the RemainingTime Entity Object.
/// It shows the remainingTime using an 4x7 segement led display
/// When the remaning time runs out the Task calls the GameOver function to suspend itself, shootcontrol, and hitprocesscontrol.
class UpdateGameTimeControl: public rtos::task<>, public IRunGameTask{
private:
	rtos::flag StartFlagTime; // is set when the game starts
	rtos::flag GameOverFlagTime; // is set when it is gameOver
	rtos::clock GameTimeClock; // clock that ticks every 100ms to tell this class to update the gameTime.

	enum UpdateGameTimeControlStates{WaitForStart, Idle}; // enum for state
	UpdateGameTimeControlStates CurrentState; // varaible to store the current State.
	int gameTime; // variable to store the current time of the game.
	int maxGameTime; // variable to store how long the game can last max.
	int startTime; // variable to store the start Time in ms.
	RemainingTime& remainingTime; // Entity to store the remaining time in.
	DigitLedDisplay& digitLedDisplay; // to display the ramaining time.
	IRunGameTask& shootControl; // a reference to the shootControl, which is to be notified when the game time is over.
	IRunGameTask& processHitControl; // a reference to the processHitControl, which is to be notified when the game time is over.
	SpeakerControl& speakerControl;
	TransferDataControl& transferDataControl;

	int msToTimeSeconds(int timeMS); // convert the remaining time in ms to displayable seconds.
	int msToTimeMinutes(int timeMS); // convert the remaining time in ms to displayable minutes.

public:
	///@fn UpdateGameTimeControl(const unsigned int priority, const char* taskName, RemainingTime& _remainingTime, DigitLedDisplay& _digitLedDisplay, IRunGameTask& _shootControl, IRunGameTask& _processHitControl, SpeakerControl& _speakerControl, TransferDataControl& _transferDataControl)
	///@brief The constructor for the UpdateGameTimeControl class.
	///@details This contructor creates a UpdateGameTimeControl object.
	///@param priority Priority of the task.
	///@param taskName Name of the task.
	///@param _remainingTime Entity object where the remaining game time is stored.
	///@param _digitLedDisplay Boundry object that control an 4x7 SEGMENT LED DISPLAY
	///@param _shootControl RTOS Task that is responsible for shooting (sending) an IR message
	///@param _processHitControl RTOS Task that is responsible for processing hits on the player
	///@param _speakerControl RTOS Task that is responsible for controlling the speaker
	///@param _transferDataControl RTOS Task that is responsible for displaying information at the end of the game
	UpdateGameTimeControl(const unsigned int priority, const char* taskName, RemainingTime& _remainingTime, DigitLedDisplay& _digitLedDisplay, IRunGameTask& _shootControl, IRunGameTask& _processHitControl, SpeakerControl& _speakerControl, TransferDataControl& _transferDataControl):
		task(priority, taskName), StartFlagTime(this, "StartFlagTime"), GameOverFlagTime(this, "GameOverFlagTime"), GameTimeClock(this, 100000, "GameTimeClock"),
		gameTime(0), maxGameTime(0), startTime(0), remainingTime(_remainingTime), digitLedDisplay(_digitLedDisplay), shootControl(_shootControl), processHitControl(_processHitControl),
		speakerControl(_speakerControl), transferDataControl(_transferDataControl){CurrentState = WaitForStart;}
		
	///@fn void UpdateGameTimeControl::Start()
	///@brief Starts this task.
	void Start();
	
	///@fn void UpdateGameTimeControl::GameOver()
	///@brief ends this task.
	void GameOver();
	
	///@fn void UpdateGameTimeControl::main()
	///@brief Starts the Task. call using rtos::run in actual the main
	void main();
	
	///@fn void UpdateGameTimeControl::SetProcessHitControl()
	///@brief Sets processHitControl
	void SetProcessHitControl(IRunGameTask& _processHitControl); // added to work around a circular reference
};
#endif
