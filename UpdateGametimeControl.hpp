#ifndef PLAYER_UPDATEGAMETIMECONTROL_HPP
#define PLAYER_UPDATEGAMETIMECONTROL_HPP

#include "rtos.hpp"

class UpdateGameTimeControl: public rtos::task<>, public IRunGameTask{
public:
	void Start();
	void GameOver();
};

#endif
