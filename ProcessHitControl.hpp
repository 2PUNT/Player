#ifndef PLAYER_PROCESSHITCONTROL_HPP
#define PLAYER_PROCESSHITCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
#include "IRunGameTask.hpp"
#include "Entities.hpp"
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

	//IRunGameTask& gameTimeControl;
	//IRunGameTask& shootControl;
	RemainingTime& time;
	HitDatas& hitdatas;
	PlayerData& playerData;

	enum class STATE {WAITING_ON_START, GAME_RUNNING};
	enum class SUBSTATE {WAITING_ON_HIT, WAITING_ON_TIMER};
	HitData hit;

	void main();
public:
	///@fn ProcessHitControl::ProcessHitControl(const unsigned int priority, const char* name)
	///@brief The constructor for the ProcessHitControl class.
	///@details This contructor creates a ProcessHitControl object.
	///@param priority Priority of the task.
	///@param name Name of the task.
	ProcessHitControl(const unsigned int priority, const char* name, RemainingTime& time, HitDatas& hitdatas, PlayerData& playerData, IRunGameTask& _gameTimeControl, IRunGameTask& _shootControl):
		task(priority, name), MessagesReceivedRunQueue(this, "MessagesReceivedRunQueue"),
		StartFlagHit(this, "startFlagHit"), GameOverFlagHit(this, "GameOverFlagHit"),
		ProcessHitTimer(this, "ProcessHitTimer"), time(time), hitdatas(hitdatas), playerData(playerData){};/*, gameTimeControl(_gameTimeControl),
		shootControl(_shootControl){};*/

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
