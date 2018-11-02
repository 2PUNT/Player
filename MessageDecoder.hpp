#ifndef PLAYER_MESSAGEDECODER_HPP
#define PLAYER_MESSAGEDECODER_HPP

#include "rtos.hpp"

// De Message ADT is niet beschreven in ADTs.hpp maar deze file gebruikt wel de Message ADT...
// De juiste file moet dus nog #include worden.

class MessageChanneler{
public:
	void SendMessage(Message m);
};

class MSGDecoderControl: public rtos::task<>{
public:
	void PauseDetected(int pauseLength);
};

#endif
