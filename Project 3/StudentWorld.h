#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include <string>
#include <map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


const int END_OF_GAME = 2000;

class Actor;
class Compiler;

// Coord class constructs as Coord(column, row)
class Coord				
{
public:
	Coord(int cc, int rr) :  m_c(cc), m_r(rr) {}
	int c() const { return m_c; }
	int r() const { return m_r; }
	
private:
	int m_c;
	int m_r;
};

bool operator< (const Coord& a, const Coord& b);

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld() {}

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// Is there a pebble at x,y
	bool isPebbleAt(int x, int y) const;

	// Is an enemy of an ant of the indicated colony at x,y?
	bool isEnemyAt(int x, int y, int colony) const;

	// Is something dangerous to an ant of the indicated colony at x,y?
	bool isDangerAt(int x, int y, int colony) const;

	// Is the anthill of the indicated colony alive at x,y?
	bool isAnthillAt(int x, int y, int colony) const;

	// Is there pheromone of the indicated colony at x,y?
	bool isPheromoneAt(int x, int y, int colony) const;

	// Is there food at x,y
	bool isFoodAt(int x, int y) const;

	// Bite a random enemy of an ant of the indicated colony at me 's location
	// (other than me; insects don't bite themselves).  Return true if an
	// enemy was bitten.
	bool biteEnemyAt(Actor* me, int colony, int biteDamage);

	// Poison all poison-able actors at x,y.
	void poisonAllPoisonableAt(int x, int y);

	// Stun all stun-able actors at x,y.
	void stunAllStunnableAt(int x, int y);

	// Record another ant birth for the indicated colony.
	void increaseScore(int colony);

	// Add pheromone of a colony at x,y
	void addPheromoneAt(int x, int y, int amt, int colony);

	// Add food of amt at x,y
	void addFoodAt(int x, int y, int amt);

	// Take food of amt from x,y
	int takeFoodAt(int x, int y, int amt);

	// Add adult grasshopper at x,y
	void addAdultGrassAt(int x, int y);

	// Add ant of a colony at x,y
	void addAntAt(int x, int y, int colony);


private:
	typedef std::vector<Actor*> VectorActor;
	typedef std::map<Coord, VectorActor> MapVectorActor;

	int m_ticks;
	int m_antsNum[4];
	Compiler *m_antsProgram[4];
	std::string m_antsName[4];
	int m_competitors;				// Number of ant colonies competing
	MapVectorActor m_actors;		//Coordinate with x col and y row
	
	bool initCompiler();
	bool initField();
	std::string statusDisplay();
	void setDisplayText();
	void updateTickCount() { ++m_ticks; }
	int getCurrentTicks() const { return m_ticks; }
	int getNumberOfAntsForAnt(int num) const { return m_antsNum[num]; }
	int getWinningAntNumber() const;
	void insertActor(Actor *ap);
	void addPebbleAt(int x, int y);
	void addBabyGrassAt(int x, int y);
	void addPoisonAt(int x, int y);
	void addWaterAt(int x, int y);
	void addAnthillAt(int x, int y, int colony);
};


#endif // STUDENTWORLD_H_
