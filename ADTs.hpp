#ifndef PLAYER_ADTS_HPP
#define PLAYER_ADTS_HPP
#include "hwlib.hpp"

struct HitData{
public:
	int8_t ShooterID;
	int HitTimeMS;
};

struct ShotData{
public:
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

#endif
