#ifndef PLAYER_PROCESSHITCONTROL_HPP
#define PLAYER_PROCESSHITCONTROL_HPP

#include "rtos.hpp"

// De Message ADT is niet beschreven in ADTs.hpp maar deze file gebruikt wel de Message ADT...
// De juiste file moet dus nog #include worden.

class ProcessHitControl: public rtos::task<>, public IRunGameTask{
public:
	void Start();
	void GameOver();
	void HitReceived(Message hit);
};

#endif
