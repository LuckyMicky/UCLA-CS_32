/*
CS 32 Project 3
Daxuan Shu
March 1, 2017
*/

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int FOOD_INIT_HP = 6000;
const int FOOD_FROM_INSECT_INIT_HP = 100;
const int PHEROMONE_INIT_HP = 256;
const int PHEROMONE_MAX_HP = 768;
const int ANTHILL_INIT_HP = 8999;
const int ANT_INIT_HP = 1500;
const int BABY_GRASSHOPPER_INIT_HP = 500;
const int ADULT_GRASSHOPPER_INIT_HP = 1600;

const int ANT_FOOD_SIZE = 100;
const int ANT_BITE_DAMAGE = 15;
const int ANT_FOOD_MAX_PER_PICKUP_SIZE = 400;
const int ANT_FOOD_MAX_PICKUP_CAPACITY = 1800;
const int ANT_HUNGRY_HP = 25;
const int ANT_MAX_INSTRUCTIONS_PER_TICK = 10;

const int ANTHILL_FOOD_MAX_SIZE = 10000;
const int ANTHILL_THRESHOLD_TO_PRODUCE_ANT = 2000;

const int GRASSHOPPER_FOOD_SIZE = 200;
const int ADULT_GRASSHOPPER_BITE_DAMAGE = 50;

const int POISON_DAMAGE = 150;
const int SLEEP_STUN_DURATION = 2;

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection, int depth);

	// Action to perform each tick. 
	virtual void doSomething() = 0;

	// Is this actor dead?
	// By default, an actor is not dead.
	virtual bool isDead() const { return false; }

	// Does this actor block movement?
	// By default, an actor is not a pebble.
	virtual bool isPebble() const { return false; }

	// Actor has recently moved if possible.
	virtual void justMoved() {}

	// Actor is ready to move the next tick.
	virtual void readyToMove() {}

	// Cause this actor to be bitten, suffering an amount of damage.
	virtual void getBitten(int amt) {}

	// Cause this actor to be poisoned.
	virtual void getPoisoned() {}

	// Cause this actor to be stunned.
	virtual void getStunned() {}

	// Adjust this actor's amount of energy upward or downward.
	virtual void updateEnergy(int amt) {}

	// Get the energy in energy holder
	virtual int getEnergy() { return 0; }

	// Can this actor be picked up to be eaten?
	// By default, an actor is not food.
	virtual bool isFood() const { return false; }

	// Is this actor detected by an ant as a pheromone of its colony?
	// By default, an actor is not pheromone.
	virtual bool isPheromone(int colony) const { return false; }

	// Is this actor an enemy of an ant of the indicated colony?
	// By default, an actor is not an enemy.
	// By default, an insect is an enemy.
	virtual bool isEnemy(int colony) const { return false; }

	// Is this actor detected as dangerous by an ant of the indicated colony?
	// By default, an actor is not dangerous.
	virtual bool isDangerous(int colony) const { return false; }

	// Is this actor the anthill of the indicated colony?
	// By default, an actor is not an anthill.
	virtual bool isAnthill(int colony) const { return false; }

	// Is there a pebble at x,y?
	bool isPebbleAt(int x, int y) const;

	// Is there an enemy of an actor's colony at x,y?
	// For Ants, enemy is an ant from different colony or a grasshopper
	bool isEnemyAt(int x, int y, int colony) const;

	// Is there danger at x,y
	// For Ants, danger is an enemy ant, grasshopper, poison or water
	bool isDangerAt(int x, int y, int colony) const;

	// Is the anthill of the indicated colony at x,y?
	bool isAnthillAt(int x, int y, int colony) const;

	// Is there pheromone of the indicated colony at x,y?
	bool isPheromoneAt(int x, int y, int colony) const;

	// Is there food at x,y
	bool isFoodAt(int x, int y) const;

	// Add pheromone of amt of a colony at x,y
	void addPheromoneAt(int x, int y, int amt, int colony);

	// Add food of amt at x,y
	void addFoodAt(int x, int y, int amt);

	// Take food of amt from x,y
	int takeFoodAt(int x, int y, int amt);

	// Add adult grasshopper at x,y
	void addAdultGrassAt(int x, int y);

	// Add ant of a colony at x,y
	void addAntAt(int x, int y, int colony);

	// Bite an enemy of an ant of the indicated colony at me 's location
	// (other than me; insects don't bite themselves).  Return true if an
	// enemy was bitten.
	// Colony number for other insects is -1, to differentiate from ants
	bool biteEnemyAt(Actor* me, int colony, int biteDamage);

	// Poison all poison-able actors at x,y.
	void poisonAllPoisonableAt(int x, int y);

	// Stun all stun-able actors at x,y.
	void stunAllStunnableAt(int x, int y);

	// Record another ant birth for the indicated colony.
	void increaseScore(int colony);

	// Has this actor moved in current tick?
	// By default, an actor has already moved.
	virtual bool hasMoved() const { return true; }

	//bool operator==(const Actor& obj) const { return (this == &obj); }

private:
	StudentWorld* m_world;
};


class Pebble : public Actor
{
public:
	Pebble(StudentWorld* sw, int startX, int startY);

	virtual void doSomething() {};
	virtual bool isPebble() const { return true; }
};


class EnergyHolder : public Actor
{
public:
	EnergyHolder(StudentWorld* sw, int imageID, int startX, int startY, Direction startDir, int depth, int HP);
	virtual bool isDead() const { return (m_hitpoints <= 0); }
	void doSomething();
	virtual void justMoved() { m_moved = true; }
	virtual void readyToMove() { m_moved = false; }
	virtual bool hasMoved() const { return m_moved; }

	// Perform actions during the tick
	virtual void doSomethingFirst() = 0;

	// Add an amount of food to this actor's location.
	void addFood(int amt);

	// Take an amount of food from this actor's location.
	int takeFood(int amt);

	// Have this actor pick up an amount of food and eat it.
	int pickupAndEatFood(int amt);

	// Get this actor's amount of energy (for a Pheromone, same as strength).
	int getEnergy() const { return m_hitpoints; }

	virtual void updateEnergy(int amt) { m_hitpoints += amt; }


private:
	int m_hitpoints;
	bool m_moved;
};


class Food : public EnergyHolder
{
public:
	Food(StudentWorld* sw, int startX, int startY, int HP);
	virtual void doSomethingFirst() {};
	virtual bool isFood() const { return true; }
};


class Anthill : public EnergyHolder
{
public:
	Anthill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
	virtual void doSomethingFirst();
	virtual bool isAntHill(int colony) const { return (m_colony == colony); }

private:
	int m_colony;
	Compiler* m_program;
};


class Pheromone : public EnergyHolder
{
public:
	Pheromone(StudentWorld* sw, int startX, int startY, int antColonyNum);
	virtual void doSomethingFirst();
	virtual bool isPheromone(int colony) const { return (colony == m_colony); }

private:
	int m_colony;

	// Gets the imageID of pheromone of an ant colony
	int getImageIDOf(int antColonyNum) const;

};


class TriggerableActor : public Actor
{
public:
	TriggerableActor(StudentWorld* sw, int imageID, int x, int y);
	virtual bool isDangerous(int colony) const { return true; }
};


class WaterPool : public TriggerableActor
{
public:
	WaterPool(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};


class Poison : public TriggerableActor
{
public:
	Poison(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};


class Insect : public EnergyHolder
{
public:
	Insect(StudentWorld* sw, int imageID, int startX, int startY, int HP);
	void doSomethingFirst();
	virtual void doSomethingMore() = 0;			//Perform other actions
	virtual void getBitten(int amt);
	virtual void getPoisoned();
	virtual void getStunned();
	virtual bool isEnemy(int colony) const { return true; }

	// Is the insect immune to being stunned?
	bool isImmuneToStunned() const { return m_immuneToStun; }

	// Is the insect stunned?
	bool isStunned() const { return (m_sleepTicks != 0); }

	// Set x,y to the coordinates of the spot one step in front of this insect.
	// Postconditions: 0 <= x < VIEW_WIDTH, 0 <= y < VIEW_HEIGHT
	void getXYInFrontOfMe(int& x, int& y) const;

	// Move this insect one step forward if possible, and return true;
	// otherwise, return false without moving.
	bool moveForwardIfPossible();

	// Increase the number of ticks this insect will sleep by the indicated amount.
	void increaseSleepTicks(int amt) { m_sleepTicks += amt; }

	// Returns a random direction up, down, left, right
	Direction randDirection() { return Direction(randInt(1, 4)); }

	// This insect dies, Sets HP to 0
	void dies();


private:
	bool m_immuneToStun;	// Immune to stun if recovered from stunned and have not moved yet
	int m_sleepTicks;
};


class Ant : public Insect
{
public:
	Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
	virtual void doSomethingMore();
	virtual void getBitten(int amt);
	virtual bool isEnemy(int colony) const { return (colony != m_colony); }
	//virtual bool moveForwardIfPossible();

private:
	Compiler* m_program;
	bool m_bitten;						// Was I bitten?
	bool m_blocked;						// Was I blocked?
	int m_randomNumber;
	int m_ic;
	int m_colony;
	int m_food;

	// Add an amount of pheromone to this actor's location.
	void addPheromone(int amt);

	// Gets the imageID of ant of an ant colony
	int getImageIDOf(int colony) const;

	// Have this actor pick up an amount of food.
	void pickupFood(int amt);

	// Have this actor eat an amount of food.
	void eatFood(int amt);

	// Have this actor turn 90 degrees Clockwise
	void turnClockwise();

	// Have this actor turn 90 degrees CounterClockwise
	void turnCounterClockwise();

	// Check if conditions as required of cmd
	bool checkIfConditions(Compiler::Command cmd);
};


class GrassHopper : public Insect
{
public:
	GrassHopper(StudentWorld* sw, int imageID, int startX, int startY, int HP);
	int getDistance() const { return m_distance; }
	void setDistance(int distance) { m_distance = distance; }
	virtual bool isDangerous(int colony) const { return true; }

	// Moves around
	void move();

	// Gets a random distance between 2 and 10
	int randDistance() { return randInt(2, 10); }

private:
	int m_distance;
};


class BabyGrassHopper : public GrassHopper
{
public:
	BabyGrassHopper(StudentWorld* sw, int startX, int startY);
	virtual void doSomethingMore();
};


class AdultGrassHopper : public GrassHopper
{
public:
	AdultGrassHopper(StudentWorld* sw, int startX, int startY);
	virtual void doSomethingMore();
	virtual void getStunned() {}			// AdultGrassHopper does not get stunned!
	virtual void getPoisoned(int amt) {}		// AdultGrassHopper does not get poisoned!
	virtual void getBitten(int amt);

private:
	// Jump within a circular radius of 10 if there is open square to jump to.
	void jump();
};









#endif // ACTOR_H_
