#ifndef PLAYER_PROCESSHITCONTROL_HPP
#define PLAYER_PROCESSHITCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
#include "IRunGameTask.hpp"
#include "Entities.hpp"
#include "DisplayControl.hpp"
/// @file

/// \brief
/// Task for Registering Hits from other players
/// \details
/// the function of this task is to set the playernumber, firepower, and time. After all these variables have been set this task starts the game.
/// The playernumber and firepower are set using an keypad. The playerNumber and firepower are set by pressing A or B Respectivly. Followed by a single digit (that is greater than 0).
/// The time and the signal to start the game are set by receiving an Message ADT through an rtos channel.
/// once the game has been started(a start signal has been received) this task wil be put in an eternal suspend.
class ProcessHitControl: public rtos::task<>, public IRunGameTask{
	rtos::channel<Message,10> MessagesReceivedRunQueue;
	rtos::flag StartFlagHit;
	rtos::flag GameOverFlagHit;
	rtos::timer ProcessHitTimer;

	IRunGameTask& gameTimeControl;
	IRunGameTask& shootControl;
	RemainingTime& time;
	HitDatas& hitdatas;
	PlayerData& playerData;
	DisplayControl& displayControl;

	enum class STATE {WAITING_ON_START, GAME_RUNNING};
	enum class SUBSTATE {WAITING_ON_HIT, WAITING_ON_TIMER};
	HitData hit;

	void main();
public:
	///@fn ProcessHitControl::ProcessHitControl(const unsigned int priority, const char* name, RemainingTime& time, HitDatas& hitdatas, PlayerData& playerData, IRunGameTask& _gameTimeControl, IRunGameTask& _shootControl, DisplayControl& _displayControl):
	///@brief The constructor for the ProcessHitControl class.
	///@details This contructor creates a ProcessHitControl object.
	///@param priority Priority of the task.
	///@param name Name of the task.
	///@param time Entity object where the remaining game time is stored.
	///@param hitdatas Entity object where every hit received is recorded.
	///@param playerData Entity object where the players data is stored.
	///@param _gameTimeControl RTOS Task that keeps track of the remaining time.
	///@param _gameTimeControl RTOS Task that keeps track of the remaining time.
	///@param _shootControl RTOS Task for shooting other players
	///@param _displayControl RTOS Task for Displaying information
	ProcessHitControl(const unsigned int priority, const char* name, RemainingTime& time, HitDatas& hitdatas, PlayerData& playerData, IRunGameTask& _gameTimeControl, IRunGameTask& _shootControl, DisplayControl& _displayControl):
		task(priority, name), MessagesReceivedRunQueue(this, "MessagesReceivedRunQueue"),
		StartFlagHit(this, "startFlagHit"), GameOverFlagHit(this, "GameOverFlagHit"),
		ProcessHitTimer(this, "ProcessHitTimer"),
		gameTimeControl(_gameTimeControl), shootControl(_shootControl),
		time(time), hitdatas(hitdatas), playerData(playerData), displayControl(_displayControl){};

	///@fn void ProcessHitControl::Start()
	///@brief Starts this task.
	void Start();

	///@fn void ProcessHitControl::GameOver()
	///@brief ends this task.
	void GameOver();

	///@fn void ProcessHitControl::HitReceived(Message hit)
	///@brief call this to start registering a hit.
	///@param hit The hit message.
	void HitReceived(Message hit);

};

#endif
