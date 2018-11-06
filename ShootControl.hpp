#ifndef PLAYER_SHOOTCONTROL_HPP
#define PLAYER_SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

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
	EncodeDecodeMSG& encodeDecoder;
	DisplayControl& displayControl;
	SendIrMessageControl& sendIrMessageControl;
	SpeakerControl& speakerControl;
	
	int buttonID;
public:
	ShootControl(const unsigned int priority, const char* taskName, PlayerData& _playerData, EncodeDecodeMSG& _encodeDecoder, DisplayControl& _displayControl, SendIrMessageControl& _sendIrMessageControl, SpeakerControl& _speakerControl):
		task(priority, taskName), StartFlagShoot(this, "StartFlagShoot"), PressedButtonsQueue(this, "PressedButtonsQueue"), ReleasedButtonsQueue(this, "ReleasedButtonsQueue"), GameOverFlagShoot(this, "GameOverFlagShoot"),
		ShootTimer(this, "ShootTimer"), playerData(_playerData), encodeDecoder(_encodeDecoder), displayControl(_displayControl), sendIrMessageControl(_sendIrMessageControl), speakerControl(_speakerControl)
		{currentState = WaitForStart;}
	
	void Start();
	void GameOver();
	void ButtonPressed(int ButtonID);
	void ButtonReleased(int ButtonID);
	
	void main();
};

#endif
