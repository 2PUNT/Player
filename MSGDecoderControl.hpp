#ifndef MSG_DECODER_CONTROL_HPP
#define MSG_DECODER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "EncodeDecodeMSG.hpp"
#include "ADTs.hpp"
#include "RegisterGameParamsControl.hpp"
#include "ProcessHitControl.hpp"

struct Record{
    uint16_t message;
    uint_fast64_t time;
};

class MessageChanneler{
private:
    Message m;
	RegisterGameParamsControl& RegGame;
	ProcessHitControl& HitControl;
public:
    MessageChanneler(RegisterGameParamsControl& _RegGame, ProcessHitControl& _HitControl):
	RegGame(_RegGame),
	HitControl(_HitControl)
	{};
    void SendMessage(Message m){
		hwlib::cout << "MessageChanneler: Message received\n";
        if(m.senderID == 0x00){
            RegGame.CommandReceived(m);
			hwlib::cout << "MessageChanneler: CommandReceived\n";
        }else{
            HitControl.HitReceived(m);
			hwlib::cout << "MessageChanneler: HitReceived\n";
        }
    }
};

class MSGDecoderControl : public rtos::task<>{
private:
    uint16_t m = 0;
    uint16_t mKnown;
    int n = 0;
    int p;
    Message em;
    MessageChanneler& Channeler;
    EncodeDecodeMSG & Encode;
    Record Records[10];
    rtos::channel<int,16> PauseQueue;
    rtos::timer DecoderTimer;
    enum class state_t {WAIT_FOR_PAUSE}; // TODO: Delete
    state_t STATE = state_t::WAIT_FOR_PAUSE; // TODO: Delete

    bool messageKnown(uint16_t m);
    bool check(uint16_t m);
    void knownMessage(uint16_t m);
    void main();
public:
    void PauseDetected(int n);
    MSGDecoderControl(const unsigned int priority, const char* taskName, MessageChanneler& _Channeler, EncodeDecodeMSG & Encode):
        task(priority, taskName),
		Channeler(_Channeler),
        Encode(Encode),
        PauseQueue(this, "PauseQueue"),
        DecoderTimer(this, "DecoderTimer")
    {};
};

#endif // MSG_DECODER_CONTROL_HPP
