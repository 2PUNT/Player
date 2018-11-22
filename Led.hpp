#ifndef PLAYER_LED_HPP
#define PLAYER_LED_HPP

#include "hwlib.hpp"
/// @file
/// \@brief
/// boundary object for turning a led on or off
class Led{
private:
	hwlib::pin_out& anode;
	
public:
	Led(hwlib::pin_out& _anode): anode(_anode){}
	
	void TurnOnOff(bool isOn);
};

#endif