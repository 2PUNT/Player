#include "ProcessHitControl.hpp"

void ProcessHitControl::Start(){
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
				//entry display health
				wait(StartFlagHit);
				S = STATE::GAME_RUNNING;
			}
			
			case STATE::GAME_RUNNING:{
				SUBSTATE SUB = SUBSTATE::WAITING_ON_HIT;
				
				while(true){
					switch(SUB){
						case SUBSTATE::WAITING_ON_HIT:{
							auto event = wait();
							if(event == GameOverFlagHit){
								suspend(); //end reached
							}else if(event == MessagesReceivedRunQueue){
								SUB = SUBSTATE::WAITING_ON_TIMER;
							}
							break;
						}
						
						case SUBSTATE::WAITING_ON_TIMER:{
							Message msg = MessagesReceivedRunQueue.read();
							ProcessHitTimer.set(5'000'000);
							hit.ShooterID = msg.senderID;
							hit.HitTimeMS = RemainingTime.get(); //get remainging game time
							Hitdatas.add(hit); //add hit to hitdata
							PlayerData.DecreaseHealth(msg.Data); //decrease health
							//make sound?
							//display?
							
							if(PlayerData.get_health() <= 0){
								GameTimeControl.GameOver();
								ShootControl.GameOver();
								//make sound?
								//display game_over?
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