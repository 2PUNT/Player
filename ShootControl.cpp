#include "hwlib.hpp"
#include "rtos.hpp"

#include "ShootControl.hpp"

#include "ADTs.hpp"
#include "SpeakerControl.hpp"

#include "Dummies.cpp"

void ShootControl::Start(){
	//hwlib::cout << "ShootControl: Start() called\n";
	StartFlagShoot.set();
}
void ShootControl::GameOver(){
	GameOverFlagShoot.set();
}
void ShootControl::ButtonPressed(int ButtonID){
	//hwlib::cout << "ShootControl: Receiving ButtonID\n";
	PressedButtonsQueue.write(ButtonID);
	//hwlib::cout << "ShootControl: ButtonID Received\n";
}
void ShootControl::ButtonReleased(int ButtonID){
	//ReleasedButtonsQueue.write(ButtonID);
}

void ShootControl::main(){
	//rtos::event combinedWaitsIdle = GameOverFlagShoot + PressedButtonsQueue;
	//rtos::event combinedWaitsReload = GameOverFlagShoot + ShootTimer;
	rtos::event lastEvent = GameOverFlagShoot + ShootTimer; // rtos::event does not have a default contructor....
	while(true){
		switch(currentState){
			case WaitForStart:{
				wait(StartFlagShoot);
				reloadTime = playerData.GetFirePower() * 1000000;
				shootMessage = encodeDecoder.EncodeMessage(Message(playerData.GetID(), playerData.GetFirePower())); // message composed of player's ID and Firepower.
				hwlib::cout << "Shoot message = " << hwlib::bin << shootMessage << hwlib::dec <<'\n';
				currentState = Idle;
				PressedButtonsQueue.clear();
				led.TurnOnOff(false);
				break;
			}
			case Idle:{
				//hwlib::cout << "ShootControl: Now Idle\n";
				lastEvent = wait(GameOverFlagShoot + PressedButtonsQueue);
				if(lastEvent == GameOverFlagShoot) suspend(); // end the task!
				else if(lastEvent == PressedButtonsQueue){
					lastPressedButtonID = PressedButtonsQueue.read();
					if(lastPressedButtonID != triggerButtonID) break; // do nothing
					ShootTimer.set(reloadTime);
					shotDatas.Add(ShotData(remainingTime.Get()));
					sendIrMessageControl.sendMessage(shootMessage);
					//hwlib::cout << "S\n";
					speakerControl.MakeSound(ShootSound);
					led.TurnOnOff(true);
					//hwlib::cout << "ShootControl: Making sound\n";
					currentState = Reload;
				}
				break;
			}
			case Reload:{
				//hwlib::cout << "ShootControl: Reloading\n";
				lastEvent = wait(GameOverFlagShoot + ShootTimer);
				//hwlib::cout << "ShootControl: Reloading Done\n";
				if(lastEvent == GameOverFlagShoot) suspend(); // end the task!
				else if(lastEvent == ShootTimer){
					PressedButtonsQueue.clear();
					led.TurnOnOff(false);
					currentState = Idle;
				}
				break;
			}
		}
	}
}
