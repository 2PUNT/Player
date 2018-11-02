#ifndef PLAYER_ENTITIES_HPP
#define PLAYER_ENTITIES_HPP

#include "ADTs.hpp"

class PlayerData{
private:
	int8_t ID;
	int FirePower;
	int Health;
public:
	int8_t GetID();
	void SetID(int8_t ID);
	
	int GetFirePower();
	void SetFirePower(int FirePower);
	
	int GetHealth();
	void SetHealth(int Health);
	
	void DecreaseHealth(int amount);
};

class HitDatas{
public:
	void Add(HitData hit);
	HitData* Get();
	int GetLength();
};

class RemainingTime{
public:
	void Set(int timeMS);
	int Get();
};

class ShotDatas{
public:
	void Add(ShotData hit);
	ShotData* Get();
	int GetLength();
};

#endif
