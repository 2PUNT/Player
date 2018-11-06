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
				wait(StartFlagTime);
				gameTime = remainingTime.Get();
				startTime = hwlib::now_us()/1000;
				CurrentState = Idle
			
			case Idle:
				digitLedDisplay.Display(msToTimeMinutes(gameTime), msToTimeSeconds(gameTime));
				lastEvent = wait(combinedEvents);
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
