#ifndef PLAYER_ADTS_HPP
#define PLAYER_ADTS_HPP
#include "hwlib.hpp"

struct HitData{
public:
	HitData(int8_t _ShooterID, int _HitTimeMS): ShooterID(_ShooterID), HitTimeMS(_HitTimeMS){}
	int8_t ShooterID;
	int HitTimeMS;
};

struct ShotData{
public:
	ShotData(int _ShotTimeMS): ShotTimeMS(_ShotTimeMS){}
	int ShotTimeMS;
};

struct PlayerDataCollection{
public:
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
	
	Message(int senderID = 0, int data = 0):
	senderID(senderID), data(data)
	{};
};

#endif
