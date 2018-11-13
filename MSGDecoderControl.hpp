#ifndef MSG_DECODER_CONTROL_HPP
#define MSG_DECODER_CONTROL_HPP

#include "rtos.hpp"
#include "EncodeDecodeMSG.hpp"

struct Record{
    uint16_t message;
    uint_fast64_t time;
};

class MessageChanneler{
private:
public:
    MessageChanneler(){};
    void SendMessage(Message m){
      //hwlib::cout << m.data << hwlib::endl;
    }
};

class MSGDecoderControl : public rtos::task<>{
private:
    uint16_t m = 0;
    uint16_t mKnown;
    int n = 0;
    int p;
    EncodeDecodeMSG & coder;
    MessageChanneler & msg;
    rtos::channel<int,16> PauseQueue;
    rtos::timer DecoderTimer;
    bool messageKnown(uint16_t m);
    bool check(uint16_t m);
    void knownMessage(uint16_t m);
    void main();
public:
    void PauseDetected(int n);
    MSGDecoderControl(const unsigned int priority, const char* taskName, MessageChanneler & msg, EncodeDecodeMSG & coder):
        task(priority, taskName),
        coder(coder),
        msg(msg),
        PauseQueue(this, "PauseQueue"),
        DecoderTimer(this, "DecoderTimer")
    {};
};

#endif // MSG_DECODER_CONTROL_HPP
