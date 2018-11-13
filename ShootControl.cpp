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
				currentState = Idle;
				break;
			}
			case Idle:{
				//hwlib::cout << "ShootControl: Now Idle\n";
				lastEvent = wait(GameOverFlagShoot + PressedButtonsQueue);
				if(lastEvent == GameOverFlagShoot) suspend(); // end the task!
				else if(lastEvent == PressedButtonsQueue){
					lastPressedButtonID = PressedButtonsQueue.read();
					if(lastPressedButtonID != triggerButtonID) break; // do nothing
					shotDatas.Add(ShotData(remainingTime.Get()));
					displayControl.DisplayString("Reloading", StringType::RELOAD);
					sendIrMessageControl.sendMessage(shootMessage);
					//hwlib::cout << "ShootControl: Making sound\n";
					speakerControl.MakeSound(ShootSound);
					ShootTimer.set(reloadTime);
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
					displayControl.Clear(StringType::RELOAD);
					currentState = Idle;
				}
				break;
			}
		}
	}
}
/* 

class ShootControl: public rtos::task<>, public IRunGameTask{
private:
	rtos::flag StartFlagShoot;
	rtos::channel<int, 5> PressedButtonsQueue;
	rtos::channel<int, 5> ReleasedButtonsQueue;
	rtos::flag GameOverFlagShoot;
	rtos::timer ShootTimer;
	
	enum ShootControlStates = { WaitForStart, Idle, Reload };
	ShootControlStates currentState;
	
	PlayerData& playerData;
	ShotDatas& shotDatas;
	RemainingTime& remainingTime;
	EncodeDecodeMSG& encodeDecoder;
	DisplayControl& displayControl;
	SendIrMessageControl& sendIrMessageControl;
	SpeakerControl& speakerControl;
	
	uint8_t triggerButtonID;
	
	int lastPressedButtonID;
	int lastReleasedButtonID;
	int reloadTime; // time to reload in ms
	uint16_t shootMessage;
public:
	ShootControl(const unsigned int priority, const char* taskName, PlayerData& _playerData, EncodeDecodeMSG& _encodeDecoder, DisplayControl& _displayControl, SendIrMessageControl& _sendIrMessageControl, SpeakerControl& _speakerControl, uint8_t _triggerButtonID):
		task(priority, taskName), StartFlagShoot(this, "StartFlagShoot"), PressedButtonsQueue(this, "PressedButtonsQueue"), ReleasedButtonsQueue(this, "ReleasedButtonsQueue"), GameOverFlagShoot(this, "GameOverFlagShoot"),
		ShootTimer(this, "ShootTimer"), playerData(_playerData), encodeDecoder(_encodeDecoder), displayControl(_displayControl), sendIrMessageControl(_sendIrMessageControl), speakerControl(_speakerControl), triggerButtonID(_triggerButtonID);
		{currentState = WaitForStart;}
	
	void Start();
	void GameOver();
	void ButtonPressed(int ButtonID);
	void ButtonReleased(int ButtonID);
	
	void main();
};
 */