#include "ProcessHitControl.hpp"

void ProcessHitControl::Start(){
	hwlib::cout << "ProcessHitControl: Start() called\n";
	StartFlagHit.set();
}

void ProcessHitControl::GameOver(){
	GameOverFlagHit.set();
}

void ProcessHitControl::HitReceived(Message hit){
	MessagesReceivedRunQueue.write(hit);
}

void ProcessHitControl::main(){
	 STATE S = STATE::WAITING_ON_START;
	 
	 while(true){
		switch(S){
			case STATE::WAITING_ON_START:{
				wait(StartFlagHit);
				S = STATE::GAME_RUNNING;
				MessagesReceivedRunQueue.clear();
				displayControl.DisplayString(playerData.GetHealth(), StringType::HEALTH);
			}
			
			case STATE::GAME_RUNNING:{
				SUBSTATE SUB = SUBSTATE::WAITING_ON_HIT;
				
				while(true){
					switch(SUB){
						case SUBSTATE::WAITING_ON_HIT:{
							auto event = wait(GameOverFlagHit + MessagesReceivedRunQueue);
							if(event == GameOverFlagHit){
								suspend(); //end reached
							}else if(event == MessagesReceivedRunQueue){
								SUB = SUBSTATE::WAITING_ON_TIMER;
							}
							break;
						}
						
						case SUBSTATE::WAITING_ON_TIMER:{
							// TODO: Make a hit sound!!
							Message msg = MessagesReceivedRunQueue.read();
							ProcessHitTimer.set(5'000'000);
							hit.ShooterID = msg.senderID;
							hwlib::cout << "ProcessHitControl: hit received, sender: " << msg.senderID << '\n';
							hit.HitTimeMS = time.Get(); //get remainging game time
							hitdatas.Add(hit); //add hit to hitdata
							playerData.DecreaseHealth(msg.data); //decrease health
							displayControl.DisplayString(playerData.GetHealth(), StringType::HEALTH);
							
							if(playerData.GetHealth() <= 0){
								gameTimeControl.GameOver();
								shootControl.GameOver();
								suspend(); //end reached
							}
							
							auto event = wait();
							if(event == GameOverFlagHit){
								suspend(); //end reached
							}else if(event == ProcessHitTimer){
								MessagesReceivedRunQueue.clear();
								SUB = SUBSTATE::WAITING_ON_HIT;
							}
							break;
						}
					}
				}
			}
		 }
	 }
}