#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

////////////////////////////////////////////////////class Actor /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth):GraphObject(imageID, startX, startY, startDir, depth)
{
    Alive = true;
    isPoisoned = false;
    isStunned = false;
    m_colony = -1;
    m_world = world;
}

bool Actor::isDead() const
{
    return !Alive;
}

bool Actor::blocksMovement() const
{
    return false;
}

void Actor::setDead()
{
    Alive = false;
}

void Actor::getPoisoned()
{
    isPoisoned = true;
}

void Actor::getStunned()
{
    isStunned = true;
}

bool Actor::isEdible() const
{
    return false;
}

bool Actor::isPheromone() const
{
    return false;
}

bool Actor::isEnemy(int colony) const
{
    if (colony == m_colony)
        return false;
    return true;
}

bool Actor::isDangerous(int colony) const
{
    return false;
}

bool Actor::isAntHill(int colony) const
{
    return true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

int Actor::getmColony() const
{
    return m_colony;
}

void Actor::setmColony(int a)
{
    m_colony = a;
}

bool Actor::isCC() const
{
    return isStunned;
}

int Actor::getEnergy() const
{
    return m_hit_points;
}

void Actor::getBitten(int amt)
{
    m_hit_points -= amt;
}

////////////////////////////////////////////////////class Pebble /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Pebble::Pebble(StudentWorld* sw, int startX, int startY):Actor(sw, startX, startY, right, IID_ROCK, 1)
{}

void Pebble::doSomething()
{
    return;
}

bool Pebble::blocksMovement() const
{
    return true;
}

////////////////////////////////////////////////////class EnergyHolder /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int dept): Actor(sw, startX, startY, startDir, imageID, dept)
{
    m_food = 0;
}

bool EnergyHolder::isDead() const
{
    return Actor::isDead();
}

int EnergyHolder::getEnergy() const
{
    return Actor::getEnergy();
}

void EnergyHolder::updateEnergy(int amt)
{
    Actor::getBitten(-amt);
}

void EnergyHolder::addFood(int amt)                                         /////////!!!!!!!!!
{
    Food* food = new Food(this->getWorld(), this->getX(), this->getY(), amt);
    StudentWorld* m = this->getWorld();
    m->addActor(food);
}

void EnergyHolder::pickupFood(int amt)
{
    m_food += amt;
}

void EnergyHolder::pickupAndEatFood(int amt)
{
    Actor::getBitten(-amt);
}

bool EnergyHolder::becomesFoodUponDeath() const
{
    return false;
}

void EnergyHolder::getBitten(int amt)
{
    Actor::getBitten(amt);
}



////////////////////////////////////////////////////class Food//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Food::Food(StudentWorld* sw, int startX, int startY, int energy):EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
    
}

void Food::doSomething()
{
    return;
}

bool Food::isEdible() const
{
    return true;
}



////////////////////////////////////////////////////class AntHill///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program):EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2)
{
     setmColony(colony);
}

void AntHill::doSomething()
{
    return;
}

bool AntHill::isMyHill(int colony) const
{
    if (getmColony() == colony)
        return true;
    return false;
}

////////////////////////////////////////////////////class Pheromone/////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony):EnergyHolder(sw, startX, startY, right, 256, IID_FOOD, 2)
{
    
}

void Pheromone::doSomething()
{
        this->getBitten(1);
        if (getEnergy() <= 0)
            setDead();
}

bool Pheromone::isPheromone(int colony) const
{
    return false;
}

void Pheromone::increaseStrength()
{
    updateEnergy(256);
    if (getEnergy() >= 768)
        updateEnergy(768 - getEnergy());
}

////////////////////////////////////////////////////class TriggerableActor//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID):Actor(sw, x, y, right, imageID, 2)
{
    
}

bool TriggerableActor::isDangerous(int colony) const
{
    return true;
}

////////////////////////////////////////////////////class WaterPool/////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WaterPool::WaterPool(StudentWorld* sw, int x, int y): TriggerableActor(sw, x, y, IID_WATER_POOL)
{
    
}

void WaterPool::doSomething()
{
    
    return;
}

////////////////////////////////////////////////////class Poison////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Poison::Poison(StudentWorld* sw, int x, int y):TriggerableActor(sw, x, y, IID_POISON)
{
    
}

void Poison::doSomething()
{
    return;
}


////////////////////////////////////////////////////class Insect////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID):EnergyHolder(world, startX, startY, random_direction(), energy, imageID, 1)
{
    stun_index = 0;
}

void Insect::doSomething()
{
    return;
}

void Insect::getBitten(int amt)
{
    EnergyHolder::getBitten(amt);
}

void Insect::getPoisoned()
{
    return;
}

void Insect::getStunned()
{
    stun_index--;
    Actor::getStunned();
}

bool Insect::isEnemy(int colony)
{
    return false;
}

bool Insect::becomesFoodUponDeath() const
{
    return true;
}

void Insect::getXYInFrontOfMe(int &x, int &y) const
{
//    Direction d = this->getDirection();
//    switch (d) {
//        case GraphObject::up:
//            
//            break;
//        case <#expression#>:
//            
//        default:
//            break;
//    }
    return;
}

bool Insect::moveForwardIfPossible()
{
    return true;
}

void Insect::increaseSleepTicks(int amt)
{
    stun_index += amt;
}

void Insect::setSleepTicks(int amt)
{
    stun_index = amt;
}

////////////////////////////////////////////////////class Ant///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID):Insect(sw, startX, startY, 1500, imageID)
{
    
}

void Ant::doSomething()
{
    return;
}

void Ant::getBitten(int amt)
{
    return;
}

bool Ant::isEnemy(int colony) const
{
    return false;
}

bool Ant::moveForwardIfPossible()
{
    return true;
}

////////////////////////////////////////////////////class Grasshopper///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID):Insect(sw, startX, startY, energy, imageID)
{
    m_distance = randInt(2, 10);
    this->setSleepTicks(2);
}

void Grasshopper::doSomething()
{
    this->getBitten(1);
    if (getEnergy() == 0)
    {
        this->addFood(100);
        this->setDead();
        return;
    }
    if (this->isCC())
    {
        this->increaseSleepTicks(-1);
        return;
    }
    if (this->getEnergy() >= 1600)
    {
        AdultGrasshopper* ad = new AdultGrasshopper(this->getWorld(), this->getX(), this->getY());
        StudentWorld* m = this->getWorld();
        m->addActor(ad);
        this->setDead();
        this->addFood(100);
        return;
    }
    int x = this->getX();
    int y = this->getY();
    Actor* cur_food = this->getWorld()->getEdibleAt(x, y);
    if (cur_food != nullptr)
    {
        if (cur_food->getEnergy() <= 200)
        {
            cur_food->getBitten(cur_food->getEnergy());
            this->pickupFood(cur_food->getEnergy());
            int i = randInt(0, 1);
            if (i == 0)
            {
                this->setSleepTicks(2);
            }
        }
        else
        {
            cur_food->getBitten(200);
            this->pickupFood(200);
            int i = randInt(0, 1);
            if (i == 0)
            {
                this->setSleepTicks(2);
            }

        }
    }
        if (m_distance == 0)
        {
            this->setDirection(random_direction());
            m_distance = randInt(2, 10);
        }
            
//            Direction d = this->getDirection();
//            switch (d) {
//                case up:
//                {
//                    if (this->getWorld()->canMoveTo(this->getX(), this->getY()+1))
//                    {
//                        m_distance--;
//                        this->moveTo(this->getX(), this->getY() + 1);
//                        break;
//                    }
//                    else
//                        m_distance = 0;
//                }
//                case down:
//                {
//                    if (this->getWorld()->canMoveTo(this->getX(), this->getY()-1))
//                    {
//                        m_distance--;
//                        this->moveTo(this->getX(), this->getY() - 1);
//                        break;
//                    }
//                    else
//                        m_distance = 0;
//
//
//                }
//                case left:
//                {
//                    if (this->getWorld()->canMoveTo(this->getX() - 1, this->getY()))
//                    {
//                        m_distance--;
//                        this->moveTo(this->getX() - 1, this->getY());
//                        break;
//                    }
//                    else
//                        m_distance = 0;
//
//
//                }
//                case right:
//                {
//                    if (this->getWorld()->canMoveTo(this->getX() + 1, this->getY()))
//                    {
//                        m_distance--;
//                        this->moveTo(this->getX() + 1, this->getY());
//                        break;
//                    }
//                    else
//                        m_distance = 0;
//
//
//                }
//            }
    
    
}

int Grasshopper::getDistance()
{
        return m_distance;
}

void Grasshopper::increDistance(int amt)
{
    m_distance += amt;
}

    

////////////////////////////////////////////////////class BabyGrasshopper///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY):Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{
}



bool BabyGrasshopper::isEnemy(int colony) const
{
    return true;
}

////////////////////////////////////////////////////class AdultGrasshopper//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY):Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER)
{
    
}

void AdultGrasshopper::getBitten(int amt)
{
    EnergyHolder::getBitten(amt);
}

//BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* world, Direction dir, int depth, double size, int imageID):Actor(imageID, startX, startY, world, dir, depth, size)
//{
//    m_hit_points = 500;
//    isDead = false;
//    crowd_control = false;
//    stun_index = 0;
//}
//
//
//void BabyGrasshopper::doSomething()
//{
//    m_hit_points--;
//    if (m_hit_points == 0)
//    {
//        double x,y;
//        this->getAnimationLocation(x, y);
//        int m = roundAwayFromZero(x);
//        int n = roundAwayFromZero(y);
//        StudentWorld* p = this->getMyWorld();
//        // cout << m << " " << n; // 11 55
//        Food  *carcass = new Food(m,n,p);
//        p->insertNewActor(m, n, carcass);
//        isDead = true;
//        return;
//    }
//    else
//    {
//        if (this->stun_index != 0)
//        {
//            this->stun_index--;
//            return;
//        }
//        else
//        {
//            if (m_hit_points >= 1600)
//            {
//                double x,y;
//                this->getAnimationLocation(x, y);
//                int m = roundAwayFromZero(x);
//                int n = roundAwayFromZero(y);
//                StudentWorld* p = this->getMyWorld();
//                AdultGrasshopper *moulted = new AdultGrasshopper(m,n,p);
//                p->insertNewActor(m, n, moulted);
//                isDead = true;
//            }
//        }
//    }
//    
//    
//}
