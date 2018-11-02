#ifndef PLAYER_TRANSFERDATA_HPP
#define PLAYER_TRANSFERDATA_HPP

#include "rtos.hpp"
#include "ADTs.hpp"

class TransferDataControl: public rtos::task<>{
public:
	void StartTransfer();
};

class PersonalComputer{
public:
	void RegisterData(PlayerDataCollection playerData);
};

#endif
