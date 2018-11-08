#include "RegisterGameParamsControl.hpp"


void RegisterGameParamsControl::CommandReceived(Message Command){
	MessagesReceivedRegQueue.write(Command);
}

void RegisterGameParamsControl::KeyboardKeyPressed(char Key){
	KeyPressedRegQueue.write(Key);
}

void RegisterGameParamsControl::main(){
	STATE S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
	
	while(true){
		switch(S){
			case STATE::WAITING_ON_COMMAND_OR_USER_INPUT:
			{
				auto event_t = wait();
				if(event_t == KeyPressedRegQueue){
					char Key = KeyPressedRegQueue.read();
					if(Key == 'A'){
						S = STATE::WAITING_FOR_PLAYERNUMBER;
						break;
					}else if(Key == 'B'){
						S = STATE::WAITING_FOR_FIREPOWER;
						break;
					}else{
						break;
					}
					
				}else if(event_t == MessagesReceivedRegQueue){
					auto msg = MessagesReceivedRegQueue.read();
					if(msg.data != 0){
						//remainingTime.set(msg.data);
						hwlib::cout << "Time set";
						TimeCheck = 1;
						break;
					}else if((msg.data == 0) && TimeCheck && PlayerIDCheck && FirePowerCheck){
						//processHitControl.start();
						//gameTimeControl.start();
						//shootControl.start();
						hwlib::cout<< "start";
						suspend();
					}else{
						break;
					}
				}
				break;
			}
			case STATE::WAITING_FOR_PLAYERNUMBER:
			{
				wait(KeyPressedRegQueue);
				char Key = KeyPressedRegQueue.read();
				if((Key > '0') && (Key < ':')){
					S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
					//playerData.setID(Key);
					hwlib::cout<< "PlayerData = " << Key << "\n";
					PlayerIDCheck = 1;
				}
				break;
			}
			case STATE::WAITING_FOR_FIREPOWER:
			{
				wait(KeyPressedRegQueue);
				char Key = KeyPressedRegQueue.read();
				if((Key > '0') && (Key < ':')){
					S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
					//playerData.setFirepower(Key);
					hwlib::cout<< "Firepower = " << Key << "\n";
					FirePowerCheck = 1;
				}
				break;
			}
		}
	}
}