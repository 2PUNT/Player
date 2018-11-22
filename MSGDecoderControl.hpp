#ifndef MSG_DECODER_CONTROL_HPP
#define MSG_DECODER_CONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "EncodeDecodeMSG.hpp"
#include "ADTs.hpp"
#include "RegisterGameParamsControl.hpp"
#include "ProcessHitControl.hpp"

#define RECORD_COLLECTION_LENGTH 20


///@struct Record
///@brief ADT for saving a record message and it's time
struct Record{
    uint16_t message;
    uint_fast64_t time;
};


///@struct RecordCollection
///@brief heap of sorts for RecordCollection
struct RecordCollection {

  Record  Records [RECORD_COLLECTION_LENGTH];
  uint8_t known   [RECORD_COLLECTION_LENGTH];
  uint8_t unknown [RECORD_COLLECTION_LENGTH];

  void addRecord(Record & r){
    cleanUpRecords();
    uint8_t id = getCurrentFreeRecord();
    Records[id] = r;
  }
  
  ///@fn void cleanUpRecords()
  ///@brief checks if the storage of records can be cleared
  void cleanUpRecords(){
    uint_fast64_t removal_limit = hwlib::now_us() - 2500;
    for(uint8_t i = 0; i < RECORD_COLLECTION_LENGTH; i++){
      if(Records[i].time < removal_limit){
          unknown[i] = i;
          known[i] = 0;
      }
    }
  }

  ///@fn int getCurrentFreeRecord()
  ///@brief Returns first current free record
  ///@return @c id
  int getCurrentFreeRecord(){
    cleanUpRecords();
    uint8_t id= 0;
    for(uint8_t i = 0; i < RECORD_COLLECTION_LENGTH; i++){
      if(unknown[i] != 0){ // implying that there is an empty spot which their should be given the time window
        id = i;
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


  ///@fn void checkKnown(uint16_t message);
  ///@brief checking if the message is known
  ///@param m the message
  ///@return @c bool
  bool checkKnown(uint16_t message){
    cleanUpRecords();
    for(auto k: known){
        if(k!= 0){
          if(Records[k].message == message){
			//hwlib::cout << "message was already known: " << hwlib::bin << message << hwlib::dec <<'\n';
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
  ///@fn MessageChanneler::MessageChanneler(RegisterGameParamsControl& _RegGame, ProcessHitControl& _HitControl):
  ///@brief The constructor for the MessageChanneler class.
  ///@param _RegGame RegisterGameParamsControl
  ///@param _HitControl ProcessHitControl
  ///@details This constructor creates a HitDatas object.
    MessageChanneler(RegisterGameParamsControl& _RegGame, ProcessHitControl& _HitControl):
	RegGame(_RegGame),
	HitControl(_HitControl)
	{};

  ///@fn void SendMessage(Message m);
  ///@brief redirecting message to the right recipient (message \ command)
  ///@param m the message
    void SendMessage(Message m){
		//hwlib::cout << "MessageChanneler: Message received\n";
        if(m.senderID == 0x00){
            RegGame.CommandReceived(m);
			//hwlib::cout << "MessageChanneler: CommandReceived\n";
        }else{
            HitControl.HitReceived(m);
			//hwlib::cout << "MessageChanneler: HitReceived: " << m.data << "\n";
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
    RecordCollection Records;
    rtos::channel<int,16> PauseQueue;
    rtos::timer DecoderTimer;
    enum class state_t {WAIT_FOR_PAUSE}; // TODO: Delete
    state_t STATE = state_t::WAIT_FOR_PAUSE; // TODO: Delete

    ///@fn void messageKnown(uint16_t m);
    ///@brief checking if the message is known
    ///@param m the message
    ///@return @c bool
    bool messageKnown(uint16_t m);

    ///@fn void check(uint16_t m);
    ///@brief checking if the message is valid
    ///@param m the message
    ///@return @c bool
    bool check(uint16_t m);

    ///@fn void knownMessage(uint16_t m);
    ///@brief storing the message
    ///@param m the message
    void knownMessage(uint16_t m);

    ///@brief RTOS activity
    void main();
public:
    void PauseDetected(int n);
    ///@fn MSGDecoderControl(const unsigned int priority, const char* taskName, MessageChanneler& _Channeler, EncodeDecodeMSG & Encode):
    ///@brief The constructor for the MSGDecoderControl class.
    ///@details This contructor creates a ProcessHitControl object.
    ///@param priority Priority of the task.
    ///@param name Name of the task.
    ///@param _Channeler MessageChanneler that decides if it's a command or message
    ///@param Encode EncodeDecodeMSG that can decode the pauses to a usefull message

    MSGDecoderControl(const unsigned int priority, const char* taskName, MessageChanneler& _Channeler, EncodeDecodeMSG & Encode):
        task(priority, taskName),
		Channeler(_Channeler),
        Encode(Encode),
        PauseQueue(this, "PauseQueue"),
        DecoderTimer(this, "DecoderTimer")
    {};
};

#endif // MSG_DECODER_CONTROL_HPP
