#ifndef PLAYER_TRANSFERDATA_HPP
#define PLAYER_TRANSFERDATA_HPP

#include "rtos.hpp"
#include "ADTs.hpp"

class TransferDataControl: public rtos::task<>{
	private:
		PlayerData & pdata;
		HitDatas & hdata;
		ShotDatas & sdata;
		PersonalComputer & PC;

	public:
		TransferDataControl(
			PlayerData & pdata,
			HitDatas & hdata,
			ShotDatas & sdata,
			PersonalComputer & PC):
			pdata(pdata),
			hdata(hdata),
			sdata(sdata),
			PC(PC){}

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

class PersonalComputer{
	public:
		PersonalComputer(){
		};
		void RegisterData(PlayerDataCollection playerData){
				seperatorLine();
				print_intro(playerData.PlayerID);
				print_activities(
					playerData.PlayerFirePower,
					playerData.HitDataLength,
					playerData.ShotDataLength
				);
				seperatorLine();
				print_hitLog(playerData.HitDataArray); // can be implemented
				print_health(playerData.PlayerHealth);
		}

		void seperatorLine(){
				hwlib::cout << "========================================================" << hwlib::endl;
		}
		void print_intro(int8_t id){
			hwlib::cout << "Beste "           << id  << hwlib::endl;
			hwlib::cout << "Je hebt zojuist een potje gespeeld en je krijgt nu de voortgang van het potje te zien." << hwlib::endl;
			hwlib::cout << "allereerst krijg je de activiteiten van het potje te zien en daarna krijg je een aantal scores te zien" << hwlib::endl;
			seperatorLine();
		}


		void print_activities(int8_t PlayerFirePower,	uint16_t HitDataLength,	uint16_t ShotDataLength){
			hwlib::cout << "De activiteiten log is als volgt: " << hwlib::endl;
			hwlib::cout << "Je hebt " << ShotDataLength << "aantal keer geschoten en daardoor heb je " << ShotDataLength * PlayerFirePower << " aantal seconden moeten wachten" << hwlib::endl;
			hwlib::cout << "je bent in de volgorde geraakt door de volgende mensen in de aflopende aantallen" << hwlib::endl;
			hwlib::cout << "Je bent " << HitDataLength   << "aantal keer beschoten en daardoor heb je " << (HitDataLength * 2)  << "aantal seconden moeten wachten" << hwlib::endl; // TODO: Time defined for hit wait
		}


		void print_health(int8_t PlayerHealth){
			hwlib::cout << "tot slot was de eindstand van je health: " << PlayerHealth <<  hwlib::endl;
		}

};

#endif
