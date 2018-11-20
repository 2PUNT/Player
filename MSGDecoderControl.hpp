#ifndef MSG_DECODER_CONTROL_HPP
#define MSG_DECODER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "EncodeDecodeMSG.hpp"
#include "ADTs.hpp"
#include "RegisterGameParamsControl.hpp"
#include "ProcessHitControl.hpp"

#define RECORD_COLLECTION_LENGTH 20
struct Record{
    uint16_t message;
    uint_fast64_t time;

};

struct RecordCollection {

  Record Records[RECORD_COLLECTION_LENGTH];
  uint8_t[RECORD_COLLECTION_LENGTH] known;
  uint8_t[RECORD_COLLECTION_LENGTH] unknown;

  void addRecord(Record & r){
    cleanUpRecords();
    uint8_t id = getCurrentFreeRecord();
    Records[id] = r;
  }

  void cleanUpRecords(){
    uint_fast64_t removal_limit = hwlib::now_us() - 2500;
    for(uint8_t i = 0; i < RECORD_COLLECTION_LENGTH; i++){
      if(Records[i].time < removal_limit){
          unknown[i] = i;
          known[i] = 0;
      }
    }
  }

  int getCurrentFreeRecord(){
    cleanUpRecords();
    uint8_t id= 0;
    for(auto u: unknown){

      if(u != 0){ // implying that there is an empty spot which their should be given the time window
        id = k;
      }
      break;
    }
    if(id == 0){
      getCurrentFreeRecord(); // recursive call just to be sure.
    }
    known[id]   = id;
    unknown[id] = 0;
    return id;
  }

  bool checkKnown(Record & r){
    cleanUpRecords();
    for(auto k: known){
        if(k!= 0){
          if(Records[k].message == r.message){
            return true;
        }
      }
    }
    return false;
  }
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
    int recordLength = 0;
    std::array<int, 10> toClear;
    Message em;
    MessageChanneler& Channeler;
    EncodeDecodeMSG & Encode;
    RecordCollection Records[RECORD_COLLECTION_LENGTH];
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
