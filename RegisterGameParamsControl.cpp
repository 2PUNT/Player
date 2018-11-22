#include "RegisterGameParamsControl.hpp"


void RegisterGameParamsControl::CommandReceived(Message Command){
	//hwlib::cout << "RegisterGameParamsControl: CommandReceived: ID: " << Command.senderID << " Data: " << Command.data << '\n';
	MessagesReceivedRegQueue.write(Command);
}

void RegisterGameParamsControl::KeyboardKeyPressed(char Key){
	//hwlib::cout << "RegisterGameParamsControl: KeyboardKeyPressed: Key: " << Key << '\n';
	KeyPressedRegQueue.write(Key);
}

void RegisterGameParamsControl::main(){
	//hwlib::cout << "RegisterGameParamsControl: Starting\n";
	STATE S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
	displayControl.DisplayString("A to set ID", StringType::RELOAD);
	displayControl.DisplayString("B to set Power", StringType::HEALTH);
	while(true){
		switch(S){
			case STATE::WAITING_ON_COMMAND_OR_USER_INPUT:
			{
				//hwlib::cout << "RegisterGameParamsControl: State: WAITING_ON_COMMAND_OR_USER_INPUT\n";
				auto event_t = wait(KeyPressedRegQueue + MessagesReceivedRegQueue);
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
						remainingTime.Set(msg.data * 60 * 1000);
						//hwlib::cout << "RegisterGameParamsControl: Set Time: " << remainingTime.Get() <<'\n';
						TimeCheck = 1;
						break;
					}else if((msg.data == 0) && TimeCheck && PlayerIDCheck && FirePowerCheck){
						startTimer.set(5'000'000);
						hwlib::cout << "RegisterGameParamsControl: Wait for start countdown\n";
						wait(startTimer);
						displayControl.Clear(StringType::HEALTH);
						displayControl.Clear(StringType::RELOAD);
						processHitControl.Start();
						gameTimeControl.Start();
						shootControl.Start();
						//hwlib::cout<< "start" << hwlib::endl;
						//hwlib::cout << "RegisterGameParamsControl: State: END\n";
						suspend();
					}else{
						break;
					}
				}
				break;
			}
			case STATE::WAITING_FOR_PLAYERNUMBER:
			{
				//hwlib::cout << "RegisterGameParamsControl: State: WAITING_FOR_PLAYERNUMBER\n";
				displayControl.DisplayString("Set ID", StringType::RELOAD);
				wait(KeyPressedRegQueue);
				char Key = KeyPressedRegQueue.read();
				if((Key > '0') && (Key < ':')){
					S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
					playerData.SetID(Key - '0');
					PlayerIDCheck = 1;
					//hwlib::cout << "RegisterGameParamsControl: playerID is now: " << playerData.GetID() << '\n';
					displayControl.DisplayString("A to set ID", StringType::RELOAD);
				}
				break;
			}
			case STATE::WAITING_FOR_FIREPOWER:
			{
				//hwlib::cout << "RegisterGameParamsControl: State: WAITING_FOR_FIREPOWER\n";
				displayControl.DisplayString("Set Power", StringType::HEALTH);
				wait(KeyPressedRegQueue);
				char Key = KeyPressedRegQueue.read();
				if((Key > '0') && (Key < ':')){
					S = STATE::WAITING_ON_COMMAND_OR_USER_INPUT;
					playerData.SetFirePower(Key - '0');
					FirePowerCheck = 1;
					//hwlib::cout << "RegisterGameParamsControl: Firepower is now: " << playerData.GetFirePower() << '\n';
					displayControl.DisplayString("B to set Power", StringType::HEALTH);
				}
				break;
			}
		}
	}
}