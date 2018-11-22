#ifndef PLAYER_SHOOTCONTROL_HPP
#define PLAYER_SHOOTCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Entities.hpp"
#include "ADTs.hpp"
#include "SpeakerControl.hpp"
#include "IRunGameTask.hpp"
#include "EncodeDecodeMSG.hpp"
#include "SendIrMessageControl.hpp"
#include "Led.hpp"
#include "IButtonListener.hpp"
/// @file

/// \brief
/// Task for shooting other players
/// \details
/// the function of this task is to shoot an IR message when the button is pressed.
/// the message it sends contains the players PlayerID and Firepower. Which have been encoded using EncodeDecodeMsg
class ShootControl: public rtos::task<>, public IRunGameTask, public IButtonListener {
private:
	rtos::flag StartFlagShoot;
	rtos::channel<int, 100> PressedButtonsQueue;
	/* rtos::pool<int> PressedButtonPool;
	rtos::flag PressedButtonFlag; */
	rtos::channel<int, 5> ReleasedButtonsQueue;
	rtos::flag GameOverFlagShoot;
	rtos::timer ShootTimer;

	enum ShootControlStates{ WaitForStart, Idle, Reload };
	ShootControlStates currentState;

	PlayerData& playerData;
	ShotDatas& shotDatas;
	RemainingTime& remainingTime;
	EncodeDecodeMSG& encodeDecoder;
	Led& led;
	SendIrMessageControl& sendIrMessageControl;
	SpeakerControl& speakerControl;

	uint8_t triggerButtonID;

	int lastPressedButtonID;
	int lastReleasedButtonID;
	int reloadTime; // time to reload in ms
	uint16_t shootMessage;
public:
	///@fn ShootControl(const unsigned int priority, const char* taskName, PlayerData& _playerData, ShotDatas& _shotDatas, RemainingTime& _remainingTime, EncodeDecodeMSG& _encodeDecoder, Led& _led, SendIrMessageControl& _sendIrMessageControl, SpeakerControl& _speakerControl, int triggerID)
	///@brief The constructor for the ShootControl class.
	///@details This contructor creates a ShootControl object.
	///@param priority Priority of the task.
	///@param taskName Name of the task.
	///@param _playerData Entity object where the players data is stored.
	///@param _shotDatas Entity object where the every shot fired by this player is recorded.
	///@param _remainingTime Entity object where the remaining game time is stored.
	///@param _encodeDecoder Application Logic object that is used to encode or decode a message.
	///@param _led Boundry object that controls an IR-led.
	///@param _sendIrMessageControl RTOS task that is responsible for sending the IR message
	///@param _speakerControl RTOS Task that is responsible for controlling the speaker
	///@param triggerID The ID that is assinged to the Button which is designated for shooting
	ShootControl(const unsigned int priority, const char* taskName, PlayerData& _playerData, ShotDatas& _shotDatas, RemainingTime& _remainingTime, EncodeDecodeMSG& _encodeDecoder, Led& _led, SendIrMessageControl& _sendIrMessageControl, SpeakerControl& _speakerControl, int triggerID):
		task(priority, taskName), StartFlagShoot(this, "StartFlagShoot"), PressedButtonsQueue(this, "PressedButtonsQueue"), ReleasedButtonsQueue(this, "ReleasedButtonsQueue"), GameOverFlagShoot(this, "GameOverFlagShoot"),
		ShootTimer(this, "ShootTimer"), playerData(_playerData), shotDatas(_shotDatas), remainingTime(_remainingTime),encodeDecoder(_encodeDecoder), led(_led), sendIrMessageControl(_sendIrMessageControl), speakerControl(_speakerControl), triggerButtonID(triggerID)
		{currentState = WaitForStart;}
		
	///@fn void ShootControl::Start()
	///@brief Starts this task.
	void Start();
	
	///@fn void ShootControl::GameOver()
	///@brief ends this task.
	void GameOver();
	
	///@fn void ShootControl::ButtonPressed(int ButtonID);
	///@brief Receive when button is pressed
	///@details This function writes the button ID to a channel when the button is pressed
	///@param ButtonID The ButtonID to write.
	void ButtonPressed(int ButtonID);
	
	///@fn void ShootControl::ButtonReleased(int ButtonID);
	///@brief Receive when button is released
	///@details This function writes the button ID to a channel when the button is released
	///@param ButtonID The ButtonID to write.
	void ButtonReleased(int ButtonID);
	
	///@fn void ShootControl::main()
	///@brief Starts the Task. call using rtos::run in actual the main
	void main();
};

#endif
