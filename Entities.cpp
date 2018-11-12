#include "hwlib.hpp"
#include "ADTs.hpp"
#include "Entities.hpp"

int8_t PlayerData::GetID(){
	return ID;
}
void PlayerData::SetID(int8_t _ID){
	ID = _ID;
}
int PlayerData::GetFirePower(){
	return FirePower;
}
void PlayerData::SetFirePower(int _FirePower){
	FirePower = _FirePower;
}
int PlayerData::GetHealth(){
	return Health;
}
void PlayerData::SetHealth(int _Health){
	Health = _Health;
}
void PlayerData::DecreaseHealth(int amount){
	Health -= amount;
}

void HitDatas::Add(HitData hit){
	HitDataArray[usedSpace] = hit;
	usedSpace++;
}
HitData* HitDatas::Get(){
	return HitDataArray;
}
int HitDatas::GetLength(){
	return usedSpace;
}

void RemainingTime::Set(int timeMS){
	RemainingTimeMS = timeMS;
}
int RemainingTime::Get(){
	return RemainingTimeMS;
}

void ShotDatas::Add(ShotData shot){
	ShotDataArray[usedSpace] = shot;
	usedSpace++;
}
ShotData* ShotDatas::Get(){
	return ShotDataArray;
}
int ShotDatas::GetLength(){
	return usedSpace;
}
