#ifndef PLAYER_ENTITIES_HPP
#define PLAYER_ENTITIES_HPP

#include "hwlib.hpp"
#include "ADTs.hpp"

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

#endif