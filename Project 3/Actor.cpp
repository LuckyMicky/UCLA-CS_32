#include <string>
#include <cmath>
#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include "Compiler.h"
using namespace std;

#define PI 3.14159265

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection, int depth)
	: GraphObject(imageID, startX, startY, startDirection, depth), m_world(sw) {}


Pebble::Pebble(StudentWorld* sw, int startX, int startY)
	: Actor(sw, IID_ROCK, startX, startY, right, 2) {}


EnergyHolder::EnergyHolder(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection, int depth, int HP)
	: Actor(sw, imageID, startX, startY, startDirection, depth), m_hitpoints(HP), m_moved(false) {}


Food::Food(StudentWorld* sw, int startX, int startY, int HP)
	: EnergyHolder(sw, IID_FOOD, startX, startY, right, 2, HP) {}


Anthill::Anthill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
	: EnergyHolder(sw, IID_ANT_HILL, startX, startY, right, 2, ANTHILL_INIT_HP)
	, m_colony(colony), m_program(program) {}


Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony)
	: EnergyHolder(sw, getImageIDOf(colony), startX, startY, right, 2, PHEROMONE_INIT_HP)
	, m_colony(colony) {}


TriggerableActor::TriggerableActor(StudentWorld* sw, int imageID, int x, int y)
	: Actor(sw, imageID, x, y, right, 2) {}


WaterPool::WaterPool(StudentWorld* sw, int startX, int startY)
	: TriggerableActor(sw, IID_WATER_POOL, startX, startY) {}


Poison::Poison(StudentWorld* sw, int startX, int startY)
	: TriggerableActor(sw, IID_POISON, startX, startY) {}


Insect::Insect(StudentWorld* sw, int imageID, int startX, int startY, int HP)
	: EnergyHolder(sw, imageID, startX, startY, randDirection(), 1, HP)
	, m_immuneToStun(false), m_sleepTicks(0) {}


Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
	: Insect(sw, getImageIDOf(colony), startX, startY, ANT_INIT_HP)
	, m_colony(colony), m_program(program), m_ic(0), m_food(0), m_randomNumber(0)
	, m_bitten(false), m_blocked(false) {}


GrassHopper::GrassHopper(StudentWorld* sw, int imageID, int startX, int startY, int HP)
	: Insect(sw, imageID, startX, startY, HP), m_distance(randDistance()) {}


BabyGrassHopper::BabyGrassHopper(StudentWorld* sw, int startX, int startY)
	: GrassHopper(sw, IID_BABY_GRASSHOPPER, startX, startY, BABY_GRASSHOPPER_INIT_HP) {}


AdultGrassHopper::AdultGrassHopper(StudentWorld* sw, int startX, int startY)
	: GrassHopper(sw, IID_ADULT_GRASSHOPPER, startX, startY, ADULT_GRASSHOPPER_INIT_HP) {}


bool Actor::isPebbleAt(int x, int y) const
{
	return m_world->isPebbleAt(x, y);
}

bool Actor::isEnemyAt(int x, int y, int colony) const
{
	return m_world->isEnemyAt(x, y, colony);
}

bool Actor::isDangerAt(int x, int y, int colony) const
{
	return m_world->isDangerAt(x, y, colony);
}

bool Actor::isAnthillAt(int x, int y, int colony) const
{
	return m_world->isAnthillAt(x, y, colony);
}

bool Actor::isPheromoneAt(int x, int y, int colony) const
{
	return m_world->isPheromoneAt(x, y, colony);
}

bool Actor::isFoodAt(int x, int y) const
{
	return m_world->isFoodAt(x, y);
}

void Actor::addAntAt(int x, int y, int colony)
{
	return m_world->addAntAt(x, y, colony);
}

void Actor::addAdultGrassAt(int x, int y)
{
	return m_world->addAdultGrassAt(x, y);
}

void Actor::addPheromoneAt(int x, int y, int amt, int colony)
{
	return m_world->addPheromoneAt(x, y, amt, colony);
}

void Actor::addFoodAt(int x, int y, int amt)
{
	m_world->addFoodAt(x, y, amt);
}

int Actor::takeFoodAt(int x, int y, int amt)
{
	return m_world->takeFoodAt(x, y, amt);
}

bool Actor::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
	return m_world->biteEnemyAt(me, colony, biteDamage);
}

void Actor::poisonAllPoisonableAt(int x, int y)
{
	m_world->poisonAllPoisonableAt(x, y);
}

void Actor::stunAllStunnableAt(int x, int y)
{
	m_world->stunAllStunnableAt(x, y);
}

void Actor::increaseScore(int colony)
{
	m_world->increaseScore(colony);
}

void EnergyHolder::doSomething()
{
	if (!hasMoved())
	{
		doSomethingFirst();
		justMoved();
	}
}

void EnergyHolder::addFood(int amt)
{
	addFoodAt(getX(), getY(), amt);
}

int EnergyHolder::takeFood(int amt)
{
	return takeFoodAt(getX(), getY(), amt);
}

int EnergyHolder::pickupAndEatFood(int amt)
{
	int foodTaken = takeFood(amt);
	updateEnergy(foodTaken);
	return foodTaken;
}

void Anthill::doSomethingFirst()
{
	if (isFoodAt(getX(), getY()))
	{
		pickupAndEatFood(ANTHILL_FOOD_MAX_SIZE);
		return;
	}
	if (getEnergy() >= ANTHILL_THRESHOLD_TO_PRODUCE_ANT)
	{
		addAntAt(getX(), getY(), m_colony);
		updateEnergy(-ANT_INIT_HP);
		increaseScore(m_colony);
	}
}

int Pheromone::getImageIDOf(int colony) const
{
	switch (colony)
	{
	case 0:
		return IID_PHEROMONE_TYPE0;
	case 1:
		return IID_PHEROMONE_TYPE1;
	case 2:
		return IID_PHEROMONE_TYPE2;
	case 3:
		return IID_PHEROMONE_TYPE3;
	default:
		return -1;
	}
}

void Pheromone::doSomethingFirst()
{
	updateEnergy(-1);
	// automatically set dead if reaches zero
}

void WaterPool::doSomething()
{
	stunAllStunnableAt(getX(), getY());
}

void Poison::doSomething()
{
	poisonAllPoisonableAt(getX(), getY());
}

void Insect::dies()
{
	updateEnergy(-getEnergy());
	addFood(FOOD_FROM_INSECT_INIT_HP);
}

void Insect::getBitten(int amt)
{
	cerr << "I get bitten (" << getEnergy();
	updateEnergy(-amt);
	cerr << "->" << getEnergy() << ")\n";
	if (isDead())
		addFood(FOOD_FROM_INSECT_INIT_HP);
}

void Insect::getPoisoned()
{
	cerr << "I get poisoned (" << getEnergy();
	updateEnergy(-POISON_DAMAGE);
	cerr << "->" << getEnergy() << ")\n";
	if (isDead())
		addFood(FOOD_FROM_INSECT_INIT_HP);
}

void Insect::getStunned()
{
	if (!isImmuneToStunned())
	{
		cerr << "I get stunned (" << m_sleepTicks;
		m_immuneToStun = true;
		increaseSleepTicks(SLEEP_STUN_DURATION);
		cerr << "->" << m_sleepTicks << ")\n";
	}
}

void Insect::doSomethingFirst()
{
	updateEnergy(-1);
	if (isDead())
	{
		addFood(FOOD_FROM_INSECT_INIT_HP);
		// automatically set in death state
		return;
	}
	else if (isStunned())	// is stunned or sleeping
	{
		--m_sleepTicks;
		return;
	}
	doSomethingMore();
}

void Insect::getXYInFrontOfMe(int& x, int& y) const
{
	switch (getDirection())
	{
	case up:
		x = getX();
		y = getY() + 1;
		break;

	case right:
		x = getX() + 1;
		y = getY();
		break;

	case left:
		x = getX() - 1;
		y = getY();
		break;

	case down:
		x = getX();
		y = getY() - 1;
		break;
	}
}

bool Insect::moveForwardIfPossible()
{
	int x, y;
	getXYInFrontOfMe(x, y);
	if (isPebbleAt(x, y))
		return false;
	moveTo(x, y);
	m_immuneToStun = false;			// no longer immune since insect moved.
	return true;
}

int Ant::getImageIDOf(int colony) const
{
	switch (colony)
	{
	case 0:
		return IID_ANT_TYPE0;
	case 1:
		return IID_ANT_TYPE1;
	case 2:
		return IID_ANT_TYPE2;
	case 3:
		return IID_ANT_TYPE3;
	default:
		return -1;
	}
}

void Ant::pickupFood(int amt)
{
	int foodTaken = takeFood(amt);
	int foodToTakeAway = min(foodTaken, ANT_FOOD_MAX_PICKUP_CAPACITY - m_food);
	m_food += foodToTakeAway;
	if (foodTaken > foodToTakeAway)
		addFood(foodTaken - foodToTakeAway);
}

void Ant::addPheromone(int amt)
{
	addPheromoneAt(getX(), getY(), amt, m_colony);
}

void Ant::eatFood(int amt)
{
	cerr << "I love food (" << getEnergy();
	int foodEnergy = min(m_food, amt);
	m_food -= foodEnergy;
	updateEnergy(foodEnergy);
	cerr << "->" << getEnergy() << ")\n";
}

void Ant::getBitten(int amt)
{
	Insect::getBitten(amt);
	m_bitten = true;
}

void Ant::turnClockwise()
{
	switch (getDirection())
	{
	case up:
		setDirection(right);
		break;

	case right:
		setDirection(down);
		break;

	case down:
		setDirection(left);
		break;

	case left:
		setDirection(up);
		break;
	}
}

void Ant::turnCounterClockwise()
{
	switch (getDirection())
	{
	case up:
		setDirection(left);
		break;

	case left:
		setDirection(down);
		break;

	case down:
		setDirection(right);
		break;

	case right:
		setDirection(up);
		break;
	}
}

void Ant::doSomethingMore()
{
	Compiler::Command cmd;
	for (int i = 0; i < ANT_MAX_INSTRUCTIONS_PER_TICK; ++i)
	{
		if (!m_program->getCommand(m_ic, cmd))
		{
			dies();
			return;
		}
		else
		{

			switch (cmd.opcode)
			{
			case Compiler::moveForward:
				if (moveForwardIfPossible())
				{
					m_blocked = false;
					m_bitten = false;
				}
				else
					m_blocked = true;
				++m_ic;
				return;

			case Compiler::eatFood:
				eatFood(ANT_FOOD_SIZE);
				++m_ic;
				return;

			case Compiler::dropFood:
				addFood(m_food);
				m_food = 0;
				++m_ic;
				return;

			case Compiler::bite:
				biteEnemyAt(this, m_colony, ANT_BITE_DAMAGE);
				++m_ic;
				return;

			case Compiler::pickupFood:
				pickupFood(ANT_FOOD_MAX_PER_PICKUP_SIZE);
				++m_ic;
				return;

			case Compiler::emitPheromone:
				addPheromone(PHEROMONE_INIT_HP);
				++m_ic;
				return;

			case Compiler::faceRandomDirection:
				setDirection(randDirection());
				++m_ic;
				return;

			case Compiler::rotateClockwise:
				turnClockwise();
				++m_ic;
				return;

			case Compiler::rotateCounterClockwise:
				turnCounterClockwise();
				++m_ic;
				return;

			case Compiler::generateRandomNumber:
				if (stoi(cmd.operand1) == 0)
					m_randomNumber = 0;
				else
					m_randomNumber = randInt(0, stoi(cmd.operand1));
				++m_ic;
				break;

			case Compiler::goto_command:
				m_ic = stoi(cmd.operand1);
				break;

			case Compiler::if_command:
				// if the condition of the if command is
				// is true, then go to the target position
				// in the vector; otherwise fall through to
				// the next position
				if (checkIfConditions(cmd))
					m_ic = (stoi(cmd.operand2));
				else
					++m_ic;			// just advance to the next line
				break;
			}
		}
	}

}

bool Ant::checkIfConditions(Compiler::Command cmd)
{
	switch (stoi(cmd.operand1))
	{
	case Compiler::last_random_number_was_zero:
		if (m_randomNumber == 0)
			return true;
		break;

	case Compiler::i_am_carrying_food:
		if (m_food > 0)
			return true;
		break;

	case Compiler::i_am_hungry:
		if (getEnergy() <= ANT_HUNGRY_HP)
			return true;
		break;

	case Compiler::i_am_standing_with_an_enemy:
		if (isEnemyAt(getX(), getY(), m_colony))
			return true;
		break;

	case Compiler::i_am_standing_on_food:
		if (isFoodAt(getX(), getY()))
			return true;
		break;

	case Compiler::i_am_standing_on_my_anthill:
		if (isAnthillAt(getX(), getY(), m_colony))
			return true;
		break;

	case Compiler::i_smell_pheromone_in_front_of_me:
		if (isPheromoneAt(getX(), getY(), m_colony))
			return true;
		break;

	case Compiler::i_smell_danger_in_front_of_me:
	{
		int x, y;
		getXYInFrontOfMe(x, y);
		if (isDangerAt(x, y, m_colony))
			return true;
		break;
	}

	case Compiler::i_was_bit:
		if (m_bitten)
			return true;
		break;

	case Compiler::i_was_blocked_from_moving:
		if (m_blocked)
			return true;
		break;
	}
	return false;
}

void GrassHopper::move()
{
	if (getDistance() == 0)
	{
		setDirection(randDirection());
		setDistance(randDistance());
	}
	else if (moveForwardIfPossible())
		setDistance(getDistance() - 1);
	else
		setDistance(0);

}

void BabyGrassHopper::doSomethingMore()
{
	if (getEnergy() >= ADULT_GRASSHOPPER_INIT_HP)
	{
		addAdultGrassAt(getX(), getY());
		dies();
		return;
	}
	if (pickupAndEatFood(GRASSHOPPER_FOOD_SIZE) != 0)	// Has eaten some amount of food
	{
		if (randInt(0, 1) == 0)			// 0 is not sleep, 1 is sleep 
			move();
	}
	else
		move();

	increaseSleepTicks(SLEEP_STUN_DURATION);
}

void AdultGrassHopper::doSomethingMore()
{
	if (randInt(1, 3) == 1)			// one in three chance
		biteEnemyAt(this, -1, ADULT_GRASSHOPPER_BITE_DAMAGE);
	else if (randInt(1, 10) == 1)		// one in ten chance
		jump();
	else if (pickupAndEatFood(GRASSHOPPER_FOOD_SIZE) != 0)
	{
		if (randInt(0, 1) == 0)			// 0 is not sleep, 1 is sleep 
			move();
	}
	else
		move();
	increaseSleepTicks(SLEEP_STUN_DURATION);
}

void AdultGrassHopper::getBitten(int amt)
{
	Insect::getBitten(amt);
	if (!isDead())
		if (randInt(0, 1) == 1)			// 1 in 2 chance
			biteEnemyAt(this, -1, ADULT_GRASSHOPPER_BITE_DAMAGE);
}

void AdultGrassHopper::jump()
{
	int radius, x, y;
	double radians;

	// Only try 10 times, then Adult GrassHopper is forced to give up jumping for better performance.
	for (int i = 0; i < 10; ++i)
	{
		radius = randInt(0, 10);
		radians = randInt(0, 360) * PI / 180;
		x = getX() + radius*cos(radians);
		y = getY() + radius*sin(radians);
		if (x >= 0 && x < VIEW_WIDTH && y >= 0 && y <= VIEW_HEIGHT && !isPebbleAt(x, y))
		{
			moveTo(x, y);
			return;
		}
	}

	// Discarded for performance issues.
	//vector<Coord> openSpace;
	//int radius = randInt(0, 10);
	//for (int r = 0; r < radius; ++r)
	//	for (int theta = 0; theta < 360; ++theta)
	//	{
	//		x = getX() + radius*cos(theta * PI / 180);
	//		y = getY() + radius*sin(theta * PI / 180);
	//		if (!isPebbleAt(x, y))
	//			openSpace.push_back(Coord(x, y));
	//	}
}
