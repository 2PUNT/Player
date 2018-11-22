#ifndef PLAYER_IRUNGAMETASKDUMMY_HPP
#define PLAYER_IRUNGAMETASKDUMMY_HPP

/// \brief A dummy object of IRunGameTask that does nothing.
class IRunGameTaskDummy: public IRunGameTask{
public:
	void Start(){}
	
	void GameOver(){}
};

#endif