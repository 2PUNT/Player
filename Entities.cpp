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

/*
class PlayerData{
private:
	int8_t ID;
	int FirePower;
	int Health;
public:
	PlayerData(): ID(0), FirePower(0), Health(0){}
	PlayerData(int8_t _ID, int _FirePower, int _Health): ID(_ID), FirePower(_FirePower), Health(_Health){}
	
	int8_t GetID();
	void SetID(int8_t _ID);
	
	int GetFirePower();
	void SetFirePower(int _FirePower);
	
	int GetHealth();
	void SetHealth(int _Health);
	
	void DecreaseHealth(int amount);
};

class HitDatas{
private:
	HitData HitDataArray[100];
	int usedSpace;
public:
	HitDatas(): usedSpace(0){}
	void Add(HitData hit);
	HitData* Get();
	int GetLength();
};

class RemainingTime{
private:
	int RemainingTimeMS;
public:
	RemainingTime(): RemainingTimeMS(0){}
	void Set(int timeMS);
	int Get();
};

class ShotDatas{
private:
	ShotData ShotDataArray[100]; 
	int usedSpace;
public:
	ShotDatas(): usedSpace(0){}
	void Add(ShotData shot);
	ShotData* Get();
	int GetLength();
};
*/
