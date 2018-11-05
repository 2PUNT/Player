#include "hwlib.hpp"
#include "rtos.hpp"
#include "DigitLedDisplay.hpp"
#include "Entities.hpp"
#include "ShootControl.hpp"
#include "ProcessHitControl.hpp"
#include "UpdateGameTimeControl.hpp"
#include "SpeakerControl.hpp"


int UpdateGameTimeControl::msToTimeSeconds(int timeMS){
	timeMS/1000;
	return timeMS%60;
}
int UpdateGameTimeControl::msToTimeMinutes(int timeMS){
	timeMS/60000;
	return timeMS%60;
}

void UpdateGameTimeControl::Start(){
	StartFlagTime.set();
}
void UpdateGameTimeControl::GameOver(){
	GameOverFlagTime.set();
}
void UpdateGameTimeControl::main(){
	rtos::event combinedEvents = GameOverFlagTime+GameTimeClock;
	rtos::event lastEvent = GameOverFlagTime+GameTimeClock; // not a very clean way, but rtos::event does not have an empty contructor
	while(true){
		switch(CurrentState){
			case WaitForStart:
				rtos::wait(StartFlagTime);
				gameTime = remainingTime.Get();
				startTime = hwlib::now_us()/1000;
				CurrentState = Idle
			
			case Idle:
				digitLedDisplay.Display(msToTimeMinutes(gameTime), msToTimeSeconds(gameTime));
				lastEvent = rtos::wait(combinedEvents);
				if(lastEvent == GameTimeClock){
					gameTime = (hwlib::now_us()/1000) - startTime;
					remainingTime.Set(gameTime);
					if(gameTime <= 0){
						shootControl.GameOver();
						processHitControl.GameOver();
						speakerControl.MakeSound(GameOverSound);
						suspend(); // end the task!
					}
				}else if(lastEvent == GameOverFlagTime){
					suspend(); // end the task!
				}
		}
	}
}


/* 
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
	ShootControl& shootControl; // a reference to the shootControl, which is to be notified when the game time is over.
	ProcessHitControl& processHitControl; // a reference to the processHitControl, which is to be notified when the game time is over.
	SpeakerControl& speakerControl;
	
	int msToTimeSeconds(int timeMS); // convert the remaining time in ms to displayable seconds.
	int msToTimeMinutes(int timeMS); // convert the remaining time in ms to displayable minutes.
	
public:
	UpdateGameTimeControl(RemainingTime& _remainingTime, DigitLedDisplay& _digitLedDisplay, ShootControl& _shootControl, ProcessHitControl& _processHitControl, SpeakerControl& _speakerControl):
		GameTimeClock(this, 100 MS, "GameTimeClock"), gameTime(0), startTime(0), remainingTime(_remainingTime), digitLedDisplay(_digitLedDisplay),
		shootControl(_shootControl), processHitControl(_processHitControl), speakerControl(_speakerControl){CurrentState = Idle}
	void Start();
	void GameOver();
	void main();
};
 */