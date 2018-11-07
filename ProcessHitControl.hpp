#ifndef PLAYER_PROCESSHITCONTROL_HPP
#define PLAYER_PROCESSHITCONTROL_HPP

#include "rtos.hpp"
#include "ADTs.hpp"

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
	
	void Start();
	void GameOver();
	void HitReceived(Message hit);
};

#endif
