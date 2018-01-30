#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual int init()
	{
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter.
		  // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
		return GWSTATUS_NO_WINNER;
	}

	virtual void cleanUp()
	{
	}

private:
};

#endif // STUDENTWORLD_H_
