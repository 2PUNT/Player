#include "hwlib.hpp"
#include "rtos.hpp"
//#include "DigitLedDisplay.hpp"
#include "Entities.hpp"
#include "ShootControl.hpp"
#include "ProcessHitControl.hpp"
#include "UpdateGameTimeControl.hpp"
#include "SpeakerControl.hpp"

//extern note* GameOverSound;

int UpdateGameTimeControl::msToTimeSeconds(int timeMS){
	timeMS/=1000;
	return timeMS%60;
}
int UpdateGameTimeControl::msToTimeMinutes(int timeMS){
	timeMS/=60000;
	return timeMS%60;
}

void UpdateGameTimeControl::Start(){
	StartFlagTime.set();
}
void UpdateGameTimeControl::GameOver(){
	GameOverFlagTime.set();
}

void UpdateGameTimeControl::SetProcessHitControl(IRunGameTask& _processHitControl){
	processHitControl = _processHitControl;
}

void UpdateGameTimeControl::main(){
	rtos::event combinedEvents = GameOverFlagTime+GameTimeClock;
	rtos::event lastEvent = GameOverFlagTime+GameTimeClock; // not a very clean way, but rtos::event does not have an empty contructor
	while(true){
		switch(CurrentState){
			case WaitForStart:{
				wait(StartFlagTime);
				//hwlib::cout << "UpdateGameTimeControl: Start() called\n";
				maxGameTime = remainingTime.Get();
				startTime = hwlib::now_us()/1000;
				//hwlib::cout << "gameTime: " << gameTime << " startTime: " << startTime << '\n';
				CurrentState = Idle;
				break;
			}
			case Idle:{
				//hwlib::cout << "UpdateGameTimeControl: now Idle\n";
				digitLedDisplay.Display(msToTimeMinutes(gameTime), msToTimeSeconds(gameTime));
				lastEvent = wait(combinedEvents);
				if(lastEvent == GameTimeClock){
					//hwlib::cout << "UpdateGameTimeControl: updating time from: " << gameTime;
					gameTime = maxGameTime - ((hwlib::now_us()/1000) - startTime);
					//hwlib::cout << " to: " << gameTime << '\n';
					remainingTime.Set(gameTime);
					if(gameTime <= 0){
						//hwlib::cout << "UpdateGameTimeControl: Time's up!\n";
						shootControl.GameOver();
						processHitControl.GameOver();
						speakerControl.MakeSound(GameOverSound);
						suspend(); // end the task!
					}
				}else if(lastEvent == GameOverFlagTime){
					suspend(); // end the task!
				}
				break;
			}
		}
	}
}
