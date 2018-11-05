#ifndef DISPLAYCONTROL_HPP
#define DISPLAYCONTROL_HPP

#include "rtos.hpp"

enum StringType{
	RELOAD,
	HEALTH,
	ENTIRE_SCREEN
};

class DisplayControl{
public:
	void DisplayString(char* s, StringType ID);
	void DisplayString(int, StringType ID);
	void Clear(StringType ID);
};

#endif
