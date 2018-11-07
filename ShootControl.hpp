#ifndef PLAYER_SHOOTCONTROL_HPP
#define PLAYER_SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Entities.hpp"
#include "ADTs.hpp"
#include "SpeakerControl.hpp"
#include "IRunGameTask.hpp"
#include "EncodeDecodeMSG.hpp"

#include "Dummies.cpp"

class ShootControl: public rtos::task<>, public IRunGameTask {
private:
	rtos::flag StartFlagShoot;
	//rtos::channel<int, 100> PressedButtonsQueue; // CHANGED!!!!
	rtos::pool<int> PressedButtonPool;
	rtos::flag PressedButtonFlag;
	rtos::channel<int, 5> ReleasedButtonsQueue;
	rtos::flag GameOverFlagShoot;
	rtos::timer ShootTimer;
	
	enum ShootControlStates{ WaitForStart, Idle, Reload };
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
	ShootControl(const unsigned int priority, const char* taskName, PlayerData& _playerData, ShotDatas& _shotDatas, RemainingTime& _remainingTime, EncodeDecodeMSG& _encodeDecoder, DisplayControl& _displayControl, SendIrMessageControl& _sendIrMessageControl, SpeakerControl& _speakerControl, int triggerID):
		task(priority, taskName), StartFlagShoot(this, "StartFlagShoot"), PressedButtonPool("pressedButtonPool"), /* PressedButtonsQueue(this, "PressedButtonsQueue"), */ PressedButtonFlag(this, "pressedButtonFlag"), ReleasedButtonsQueue(this, "ReleasedButtonsQueue"), GameOverFlagShoot(this, "GameOverFlagShoot"),
		ShootTimer(this, "ShootTimer"), playerData(_playerData), shotDatas(_shotDatas), remainingTime(_remainingTime),encodeDecoder(_encodeDecoder), displayControl(_displayControl), sendIrMessageControl(_sendIrMessageControl), speakerControl(_speakerControl), triggerButtonID(triggerID)
		{currentState = WaitForStart;}
	
	void Start();
	void GameOver();
	void ButtonPressed(int ButtonID);
	void ButtonReleased(int ButtonID);
	
	void main();
};

#endif
