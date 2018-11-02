#ifndef DISPLAYCONTROL_HPP
#define DISPLAYCONTROL_HPP

#include "rtos.hpp"

// hoe geef je een enum mee als paramter?

enum StringType{
	RELOAD,
	HEALTH
};

class DisplayControl{
public:
	void DisplayString(char* s, StringType ID);
	void Clear(StringType ID);
};

#endif
