#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

bool operator< (const Coord& a, const Coord& b) 
{
	return (a.r() == b.r() ? a.c() < b.c() : a.r() < b.r());
}

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir), m_ticks(0) {}


bool StudentWorld::isPebbleAt(int x, int y) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isPebble())
				return true;
	return false;
}


bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isEnemy(colony))
				return true;
	return false;
}


bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isDangerous(colony))
				return true;
	return false;
}


bool StudentWorld::isFoodAt(int x, int y) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isFood())
				return true;
	return false;
}


bool StudentWorld::isAnthillAt(int x, int y, int colony) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isAnthill(colony) && it->second[i]->getEnergy() > 0)
				return true;
	return false;
}


bool StudentWorld::isPheromoneAt(int x, int y, int colony) const
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isPheromone(colony))
				return true;
	return false;
}



bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(me->getX(), me->getY()));
	vector<Actor*> enemy;
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			if (it->second[i]->isEnemy(colony))
				enemy.push_back(it->second[i]);

	if (enemy.empty())
		return false;

	int randomEnemy = randInt(0, enemy.size() - 1);
	enemy[randomEnemy]->getBitten(biteDamage);
	return true;
}


void StudentWorld::poisonAllPoisonableAt(int x, int y)
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			it->second[i]->getPoisoned();
}


void StudentWorld::stunAllStunnableAt(int x, int y)
{
	MapVectorActor::const_iterator it = m_actors.find(Coord(x, y));
	if (it != m_actors.end())
		for (int i = 0; it->second.begin() + i < it->second.end(); i++)
			it->second[i]->getStunned();
}


void StudentWorld::increaseScore(int colony)
{
	++m_antsNum[colony];
}


void StudentWorld::addPheromoneAt(int x, int y, int amt, int colony)
{
	for (VectorActor::const_iterator it = m_actors[Coord(x, y)].begin(); it != m_actors[Coord(x, y)].end(); it++)
	{
		if ((*it)->isPheromone(colony))
		{
			// Make sure Pheromone does not max out
			int pheromoneEnergy = max(PHEROMONE_MAX_HP - (*it)->getEnergy(), amt);
			(*it)->updateEnergy(pheromoneEnergy);
			return;
		}
	}

	// If there is no pheromone of the colony at x,y, create a new one.
	insertActor(new Pheromone(this, x, y, colony));		
}


void StudentWorld::addFoodAt(int x, int y, int amt)
{
	// Precondition: Coord(x,y) exists in map
	for (VectorActor::const_iterator it = m_actors[Coord(x, y)].begin(); it != m_actors[Coord(x, y)].end(); it++)
	{
		if ((*it)->isFood())
		{
			(*it)->updateEnergy(amt);
			return;
		}
	}
	insertActor(new Food(this, x, y, amt));
}


int StudentWorld::takeFoodAt(int x, int y, int amt)
{
	for (VectorActor::const_iterator it = m_actors[Coord(x, y)].begin(); it != m_actors[Coord(x, y)].end(); it++)
	{
		if ((*it)->isFood())
		{
			int foodEnergy = min((*it)->getEnergy(),amt);	 // take the maximum amt that actor can take or whatever is left
			(*it)->updateEnergy(-foodEnergy);
			return foodEnergy;
		}
	}
	return 0;	// food not found
}


void StudentWorld::insertActor(Actor *ap)
{
	m_actors[Coord(ap->getX(), ap->getY())].push_back(ap); 
}


void StudentWorld::addPebbleAt(int x, int y)
{
	insertActor(new Pebble(this, x, y));
}


void StudentWorld::addBabyGrassAt(int x, int y) 
{
	insertActor(new BabyGrassHopper(this, x, y));
}


void StudentWorld::addAdultGrassAt(int x, int y) 
{
	insertActor(new AdultGrassHopper(this, x, y));
}


void StudentWorld::addPoisonAt(int x, int y)
{
	insertActor(new Poison(this, x, y));
}


void StudentWorld::addWaterAt(int x, int y)
{
	insertActor(new WaterPool(this, x, y));
}


void StudentWorld::addAntAt(int x, int y, int colony)
{
	insertActor(new Ant(this, x, y, colony, m_antsProgram[colony]));
}


void StudentWorld::addAnthillAt(int x, int y, int colony)
{
	insertActor(new Anthill(this, x, y, colony, m_antsProgram[colony]));
}


string StudentWorld::statusDisplay()
{
	ostringstream oss;
	oss << "Ticks:" << setw(5) << END_OF_GAME-getCurrentTicks() << " -";

	for (int i = 0; i < m_competitors; ++i)
	{
		oss << "  " << m_antsName[i];
		if (i == getWinningAntNumber())
			oss << '*';
		oss << ": " << setfill('0') << setw(2) << m_antsNum[i];
	}

	return oss.str();
}


void StudentWorld::setDisplayText()
{
	// Create a string from your statistics, of the form:
	// Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
	string s = statusDisplay();
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}


int StudentWorld::getWinningAntNumber() const
{
	if (m_competitors == 1)
		return -1;
	int iMax = 0;
	int numMax = getNumberOfAntsForAnt(0);
	for (int i = 1; i < m_competitors; ++i)
		if (getNumberOfAntsForAnt(i) > numMax)
		{
			iMax = i;
			numMax = m_antsNum[i];
		}
		else if (getNumberOfAntsForAnt(i) == numMax)
			iMax = -1;
	return iMax;
}


bool StudentWorld::initCompiler()
{
	// get the names of all of the ant program source files
	// we provide the getFilenamesOfAntPrograms() function for
	// you in our GameWorld class.
	vector<string> fileNames = getFilenamesOfAntPrograms();
	m_competitors = fileNames.size();

	string error;

	// compile the source file... If the compile function returns
	// false, there was a syntax error during compilation!
	for (int colony = 0; colony < m_competitors; ++colony)
	{
		m_antsProgram[colony] = new Compiler;
		if (!m_antsProgram[colony]->compile(fileNames[colony], error))
		{
			// entrant 0��s source code had a syntax error!
			// send this error to our framework to warn the user.
			setError(fileNames[colony] + " " + error);
			return false;
		}
		m_antsNum[colony] = 0;
		m_antsName[colony] = m_antsProgram[colony]->getColonyName();
	}
	return true;
}


bool StudentWorld::initField()
{
	Field f;
	string fieldFile = getFieldFilename();
	string error;

	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return false;		// something bad happened!
	}

	// otherwise the load was successful and you can access the
	// contents of the field 

	for (int x = 0; x < VIEW_WIDTH; x++)
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			switch (f.getContentsOf(x, y))			// x is col, y is row
			{
			case Field::FieldItem::rock:
				addPebbleAt(x, y);
				break;

			case Field::FieldItem::grasshopper:
				addBabyGrassAt(x, y);
				break;

			case Field::FieldItem::food:
				addFoodAt(x, y, FOOD_INIT_HP);
				break;

			case Field::FieldItem::poison:
				addPoisonAt(x, y);
				break;

			case Field::FieldItem::water:
				addWaterAt(x, y);
				break;

			case Field::FieldItem::anthill0:
				if (m_competitors >= 1)
					addAnthillAt(x, y, 0);
				break;

			case Field::FieldItem::anthill1:
				if (m_competitors >= 2)
					addAnthillAt(x, y, 1);
				break;

			case Field::FieldItem::anthill2:
				if (m_competitors >= 3)
					addAnthillAt(x, y, 2);
				break;

			case Field::FieldItem::anthill3:
				if (m_competitors >= 4)
					addAnthillAt(x, y, 3);
				break;

			case Field::FieldItem::empty:
				break;
			}
		}
	return true;
}


int StudentWorld::init()
{
	m_ticks = 0;

	if (initCompiler() && initField())
		return GWSTATUS_CONTINUE_GAME;
	return GWSTATUS_LEVEL_ERROR;
}


int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
	// return GWSTATUS_NO_WINNER;

	updateTickCount(); // update the current tick # in the simulation
					   // The term " actors" refers to all ants, anthills, poison, pebbles,
					   // baby and adult grasshoppers, food, pools of water, etc.
					   // Give each actor a chance to do something

	for (auto it = m_actors.begin(); it != m_actors.end();)
	{
		for (int i = 0; it->second.begin() + i < it->second.end(); )
		{
			Actor* q = it->second[i];
			// get the actor's current location
			int oldX = q->getX(), oldY = q->getY();

			if (!q->isDead())		
				q->doSomething();
			if (q->getX() != oldX || q->getY() != oldY)
			{
				insertActor(q);
				it->second.erase(it->second.begin() + i);
			}
			else
				++i;
		}
		if (it->second.empty())
			it = m_actors.erase(it);
		else
			++it;
	}
	//// Remove newly-dead actors after each tick
	//removeDeadSimulationObjects(); // delete dead simulation objects
	for (auto it = m_actors.begin(); it != m_actors.end();)
	{
		for (int i = 0; it->second.begin() + i < it->second.end(); )
		{
			Actor* q = it->second[i];
			if (q->isDead())
				it->second.erase(it->second.begin() + i);
			else
				++i;
		}
		if (it->second.empty())
			it = m_actors.erase(it);
		else
			++it;
	}

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		for (int i = 0; it->second.begin() + i < it->second.end(); ++i)
		{
			Actor* q = it->second[i];
			q->readyToMove();
		}
	}
							   // Update the simulation Status Line
	setDisplayText(); // update the ticks/ant stats text at screen top
						 // If the simulation��s over (ticks == 2000) then see if we have a winner
	if (m_ticks == END_OF_GAME)
	{
		if (getWinningAntNumber() != -1)
		{
			setWinner(m_antsName[getWinningAntNumber()]);
			return GWSTATUS_PLAYER_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
	// the simulation is not yet over, continue!
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
	auto it = m_actors.begin();
	while (!m_actors.empty())
	{
		while (it->second.begin() < it->second.end())
		{
			delete it->second.at(0);
			it->second.erase(it->second.begin());
		}
		if (it->second.empty())
			it = m_actors.erase(it);
	}
	
	for (int i = 0; i < m_competitors; ++i)
		delete m_antsProgram[i];
}


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
