#ifndef PLAYER_ADTS_HPP
#define PLAYER_ADTS_HPP
#include "hwlib.hpp"

struct HitData{
public:
	HitData(): ShooterID(0), HitTimeMS(0){}
	HitData(int8_t _ShooterID, int _HitTimeMS): ShooterID(_ShooterID), HitTimeMS(_HitTimeMS){}
	int8_t ShooterID;
	int HitTimeMS;
};

struct ShotData{
public:
	ShotData():ShotTimeMS(0){}
	ShotData(int _ShotTimeMS): ShotTimeMS(_ShotTimeMS){}
	int ShotTimeMS;
};

struct PlayerDataCollection{
public:
	PlayerDataCollection(){}
	int8_t PlayerID;
	int8_t PlayerFirePower;
	int8_t PlayerHealth;
	HitData* HitDataArray;
	uint16_t HitDataLength;
	ShotData* ShotDataArray;
	uint16_t ShotDataLength;
};

struct Message{
	uint8_t senderID;
	uint8_t data;
	
	Message():senderID(0), data(0){}
	Message(int senderID, int data):
	senderID(senderID), data(data)
	{};
};

#endif
