#ifndef PLAYER_ADTS_HPP
#define PLAYER_ADTS_HPP

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
	int PlayerFirePower;
	int PlayerHealth;
	HitData HitDataArray*;
	int HitDataLength;
	ShotData ShotDataArray*;
	int ShotDataLength;
};

#endif
