#ifndef PLAYER_ENTITIES_HPP
#define PLAYER_ENTITIES_HPP

#include "hwlib.hpp"
#include "ADTs.hpp"

///@class PlayerData
///@brief PLayerData entitiy object
class PlayerData{
private:
	int8_t ID;
	int FirePower;
	int Health;
public:
	///@fn PlayerData::PlayerData()
	///@brief The costructor for the PlayerData object.
	///@details This constructor creates a PlayerData object.
	PlayerData(): ID(0), FirePower(0), Health(0){}
	
	///@fn PlayerData::PlayerData(int8_t _ID, int _FirePower, int _Health)
	///@brief The costructor for the PlayerData object.
	///@details This constructor creates a PlayerData object.
	///@param _ID The player's ID.
	///@param _FirePower The player's fire power.
	///@param _Health The player's health.
	PlayerData(int8_t _ID, int _FirePower, int _Health): ID(_ID), FirePower(_FirePower), Health(_Health){}
	
	///@fn int8_t PlayerData::GetID()
	///@brief Returns the player's ID.
	///@return @c ID
	int8_t GetID();
	
	///@fn void PlayerData::SetID(int8_t _ID)
	///@brief Sets the player's ID.
	///@param _ID The player's ID.
	void SetID(int8_t _ID);
	
	///@fn int PlayerData
	///@brief Returns the player's fire power.
	///@return @c FirePower
	int GetFirePower();
	
	///@fn void PlayerData::SetFirePower(int _FirePower)
	///@brief Sets the player's fire power.
	///@param _FireFlower The player's fire power.
	void SetFirePower(int _FirePower);
	
	///@fn int PlayerData::GetHealth()
	///@brief Returns the player's health.
	///@return @c Health
	int GetHealth();
	
	///@fn void PlayerData::SetHealth(int _Health)
	///@brief Sets the player's health.
	///@param _Health The player's health.
	void SetHealth(int _Health);
	
	///@fn void PlayerData::DecreaseHealth(int amount)
	///@brief Decreases the player's health by a specified amount.
	///@param amount The value that's decreasing the player's health.
	void DecreaseHealth(int amount);
};

///@class HitDatas
///@brief The amount of the player's hits.
class HitDatas{
private:
	HitData HitDataArray[100];
	int usedSpace;
public:
	///@fn HitDatas::HitDatas()
	///@brief The constructor for the HitDatas class.
	///@details This constructor creates a HitDatas object.
	HitDatas(): usedSpace(0){}
	
	///@fn void HitDatas::Add(HitData hit)
	///@brief Adds the HitData to @c HitDatasArray.
	///@param hit HitData object.
	void Add(HitData hit);
	
	///@fn HitData* Get()
	///@brief Returns the pointer to @c HitDatasArray
	///@return @c HitData*
	HitData* Get();
	
	///@fn int GetLength()
	///@brief Returns the length of @c HitDatasArray
	///@return @c usedSpace
	int GetLength();
};

class RemainingTime{
private:
	int RemainingTimeMS;
public:
	///@fn RemainingTime::RemainingTime()
	///@brief The constructor for the RemainingTime class.
	///@details This constructor creates a RemainingTime object.
	RemainingTime(): RemainingTimeMS(0){}
	
	///@fn void RemainingTime::Set(int timeMS)
	///@brief This function sets the remaining time.
	///@param timeMS The remaining time.
	void Set(int timeMS);
	
	///@fn int RemainingTime::Get()
	///@brief Returns the remaining time.
	///@return @c RemainingTimeMS
	int Get();
};

class ShotDatas{
private:
	ShotData ShotDataArray[100]; 
	int usedSpace;
public:
	///@fn ShotDatas::ShotDatas()
	///@brief The constructor for the ShotDatas class.
	///@details This constructor creates a ShotDatas object.
	ShotDatas(): usedSpace(0){}
	
	///@fn void ShotDatas::Add(ShotData shot)
	///@brief Adds the shots to @c ShotDataArray
	///@param shot The player's shots
	void Add(ShotData shot);
	
	///@fn ShotData* ShotDatas::Get()
	///@brief Returns the pointer to @c ShotDataArray
	///@return @c ShotData*
	ShotData* Get();
	
	///@fn int ShotDatas::GetLength()
	///@brief Returns the length of @c ShotDataArray
	///@return @c usedSpace
	int GetLength();
};

#endif
