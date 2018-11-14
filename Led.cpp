#include "hwlib.hpp"
#include "Led.hpp"

void Led::TurnOnOff(bool isOn){
	anode.set(isOn);
}
