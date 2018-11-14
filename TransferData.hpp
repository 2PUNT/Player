#ifndef PLAYER_TRANSFERDATA_HPP
#define PLAYER_TRANSFERDATA_HPP

#include "rtos.hpp"
#include "ADTs.hpp"

class PersonalComputer{
	public:
		PersonalComputer(){};

		// entity collection assignment so that all the data can be extracted
		void RegisterData(PlayerDataCollection playerData){
				seperatorLine();
				print_intro(playerData.PlayerID);
				seperatorLine();
				print_activities(
					playerData.PlayerFirePower,
					playerData.HitDataLength,
					playerData.ShotDataLength
				);
				seperatorLine();
				print_health(playerData.PlayerHealth);
				seperatorLine();
		}

		///! prints a line to seperate the cout sections
		void seperatorLine(){
				hwlib::cout << "========================================================" << hwlib::endl;
		}

		//! @param player id needs to be given for the introduction
		void print_intro(int8_t id){
			hwlib::cout << "Beste "           << id  << ","<< hwlib::endl;
			hwlib::cout << "Je hebt zojuist een potje gespeeld en je krijgt nu de voortgang van het potje te zien." << hwlib::endl;
			hwlib::cout << "allereerst krijg je de activiteiten van het potje te zien en daarna krijg je een aantal scores te zien" << hwlib::endl;
		}

		//! @param PlayerFirePower is used to calculate the wait time for the game
		//! @param HitDataLength is used to calculate the amount of hits for the waited time
		//! @param ShotDataLength id needs to be given to know how many times have been shot
		void print_activities(int8_t PlayerFirePower,	uint16_t HitDataLength,	uint16_t ShotDataLength){
			hwlib::cout << "De activiteiten log is als volgt: " << hwlib::endl;
			hwlib::cout << "Je hebt " << ShotDataLength << " aantal keer geschoten en daardoor heb je " << ShotDataLength * PlayerFirePower << " aantal seconden moeten wachten" << hwlib::endl;
			hwlib::cout << "Je bent " << HitDataLength   << " aantal keer beschoten en daardoor heb je " << (HitDataLength * 2)  << " aantal seconden moeten wachten" << hwlib::endl; // TODO: Time defined for hit wait
		}

		//! @param PlayerHealth is used to cout the latest health of player
		void print_health(int8_t PlayerHealth){
			if(PlayerHealth > 0){
				hwlib::cout << "tot slot was de eindstand van je health: " << PlayerHealth <<  hwlib::endl;
			}else{
				hwlib::cout << "Helaas had je geen Health meer over" << hwlib::endl;
			}
		}
};

class TransferDataControl/* : public rtos::task<> */{
	private:
		PlayerData & pdata;
		HitDatas & hdata;
		ShotDatas & sdata;
		PersonalComputer & PC;

	public:
		//! Initialized with all the entity objects and the pc proxy
		TransferDataControl(
			PlayerData & pdata,
			HitDatas & hdata,
			ShotDatas & sdata,
			PersonalComputer & PC):
			pdata(pdata),
			hdata(hdata),
			sdata(sdata),
			PC(PC){}

	//! Gathers all the entity data and sends it to the PC
	void StartTransfer(){
		PlayerDataCollection playerData;
		playerData.PlayerID        = pdata.GetID();
		playerData.PlayerFirePower = pdata.GetFirePower();
		playerData.PlayerHealth    = pdata.GetHealth();
		playerData.HitDataArray    = hdata.Get();
		playerData.HitDataLength   = hdata.GetLength();
		playerData.ShotDataArray   = sdata.Get();
		playerData.ShotDataLength  = sdata.GetLength();
		PC.RegisterData(playerData);
	}
};

#endif
