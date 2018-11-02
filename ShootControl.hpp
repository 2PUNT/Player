#ifndef PLAYER_SHOOTCONTROL_HPP
#define PLAYER_SHOOTCONTROL_HPP

#include "rtos.hpp"

class ShootControl: public rtos::task<>, public IRunGameTask{
public:
	void Start();
	void GameOver();
	void ButtonPressed(int ButtonID);
	void ButtonReleased(int ButtonID);
};

#endif
