#ifndef PLAYER_PROCESSHITCONTROL_HPP
#define PLAYER_PROCESSHITCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"
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
	
	enum class STATE {WAITING_ON_START, GAME_RUNNING};
	enum class SUBSTATE {WAITING_ON_HIT, WAITING_ON_TIMER};
	HitData hit;
	
	void main();
public:
	ProcessHitControl(const unsigned int priority, const char* name):
	task(priority, name), MessagesReceivedRunQueue(this, "MessagesReceivedRunQueue"),
	StartFlagHit(this, "startFlagHit"), GameOverFlagHit(this, "GameOverFlagHit"),
	ProcessHitTimer(this, "ProcessHitTimer"){};
/// \brief
/// starts this task
	void Start();
/// \brief
/// ends this task
	void GameOver();
/// \brief
/// call this to start registering a hit
	void HitReceived(Message hit);
};

#endif
