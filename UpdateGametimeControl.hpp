#ifndef PLAYER_UPDATEGAMETIMECONTROL_HPP
#define PLAYER_UPDATEGAMETIMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "DigitLedDisplay.hpp"
#include "Entities.hpp"
#include "ShootControl.hpp"
#include "ProcessHitControl.hpp"

class UpdateGameTimeControl: public rtos::task<>, public IRunGameTask{
private:
	rtos::flag StartFlagTime; // is set when the game starts
	rtos::flag GameOverFlagTime; // is set when it is gameOver
	rtos::clock GameTimeClock; // clock that ticks every 100ms to tell this class to update the gameTime.
	
	enum UpdateGameTimeControlStates{WaitForStart, Idle}; // enum for state
	UpdateGameTimeControlStates CurrentState; // varaible to store the current State.
	int gameTime; // variable to store the current time of the game.
	int startTime; // variable to store the start Time in ms.
	RemainingTime& remainingTime; // Entity to store the remaining time in.
	DigitLedDisplay& digitLedDisplay; // to display the ramaining time.
	IRunGameTask& shootControl; // a reference to the shootControl, which is to be notified when the game time is over.
	IRunGameTask& processHitControl; // a reference to the processHitControl, which is to be notified when the game time is over.
	SpeakerControl& speakerControl;
	
	int msToTimeSeconds(int timeMS); // convert the remaining time in ms to displayable seconds.
	int msToTimeMinutes(int timeMS); // convert the remaining time in ms to displayable minutes.
	
public:
	UpdateGameTimeControl(const unsigned int priority, const char* taskName, RemainingTime& _remainingTime, DigitLedDisplay& _digitLedDisplay, IRunGameTask& _shootControl, IRunGameTask& _processHitControl, SpeakerControl& _speakerControl):
		task(priority, taskName), GameTimeClock(this, 100 MS, "GameTimeClock"), gameTime(0), startTime(0), remainingTime(_remainingTime), digitLedDisplay(_digitLedDisplay), shootControl(_shootControl), processHitControl(_processHitControl),
		speakerControl(_speakerControl){CurrentState = Idle}
	void Start();
	void GameOver();
	void main();
	void SetProcessHitControl(IRunGameTask& _processHitControl);
};
#endif
