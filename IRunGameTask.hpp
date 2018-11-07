#ifndef PLAYER_IRUNGAMETASK_HPP
#define PLAYER_IRUNGAMETASK_HPP

///@class IRunGameTask
///@brief Interface class
class IRunGameTask{
public:
	///@fn virtual void IRunGameTask::start()=0
	///@brief Starts the game.
	virtual void Start()=0;
	
	///@fn virtual void IRunGameTask::GameOver()=0
	///@brief Ends the game.
	virtual void GameOver()=0;
};

#endif
