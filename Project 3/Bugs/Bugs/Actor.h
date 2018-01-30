#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;


class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    virtual bool isDead() const;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    virtual void setDead();
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone() const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isAntHill(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
    
       virtual int getmColony() const;
    
    virtual void setmColony(int a);
    
    static Direction random_direction()
        {
            int i = randInt(0, 3);
            switch (i) {
                case 0:
                    return up;
                    break;
                case 1:
                    return right;
                    break;
                case 2:
                    return down;
                    break;
                case 3:
                    return left;
                    break;
                default:
                    return none;
                    break;
            }
        }
    //static string colonyID
    
    //return true is the Actor has any debuff like stunned and sleeping
    virtual bool isCC() const;
    
    // return Actor's hit_points
    virtual int getEnergy() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
private:
        int m_hit_points;
        bool Alive;
        bool isPoisoned;
        bool isStunned;
        int m_colony;
        StudentWorld* m_world;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    virtual bool isDead() const;
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food.
    void pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    void pickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
    virtual bool becomesFoodUponDeath() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
private:
    int m_food;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isMyHill(int colony) const;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    virtual bool becomesFoodUponDeath() const;
    
    // Set x,y to the coordinates of the spot one step in front of this insect.
    void getXYInFrontOfMe(int &x, int &y) const;
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
    virtual bool moveForwardIfPossible();
    
    // Increase the number of ticks this insect will sleep by the indicated amount.
    void increaseSleepTicks(int amt);
    void setSleepTicks(int amt);
private:
    int stun_index;
};

class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
    virtual bool moveForwardIfPossible();

};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
    int getDistance();
    virtual void increDistance(int amt);
private:
    int m_distance;
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual bool isEnemy(int colony) const;
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void getBitten(int amt);
};

//class Actor : public GraphObject
//{
//public:
//    Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 0, double size = 0.25):GraphObject(imageID, startX, startY, dir, depth, size)
//    {
//        m_hit_points = 0;
//        Alive = false;
//        crowd_control = false;
//        stun_index = 0;
//        m_world = world;
//    }
//    
//    virtual void doSomething() = 0;
//    
//    static Direction random_direction()
//    {
//        int i = randInt(0, 3);
//        switch (i) {
//            case 0:
//                return up;
//                break;
//            case 1:
//                return right;
//                break;
//            case 2:
//                return down;
//                break;
//            case 3:
//                return left;
//                break;
//            default:
//                return none;
//                break;
//        }
//    }
//    
//
//        bool isAlive() const
//    {
//        return !isDead;
//    }
//    
//    void setDead()
//    {
//        isDead = true;
//    }
//    
//    int get_hit_points() const
//    {
//        return m_hit_points;
//    }
//    
//    void set_hit_points(int amount, bool in_or_de)
//    {
//        if (in_or_de ==true)
//            m_hit_points += amount;
//        else
//            m_hit_points -= amount;
//    }
//    
//    StudentWorld* getMyWorld()
//    {
//        return m_world;
//    }
//    
//private:
//    int m_hit_points;
//    bool Alive;
//    bool crowd_control;
//    int stun_index;
//    StudentWorld* m_world;
//};
//
//class Pebble : public Actor
//{
//public:
//    Pebble(int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 1,  double size = 0.30, int imageID = IID_ROCK):Actor(IID_ROCK, startX, startY, world, dir, depth, size){}
//    
//    virtual void doSomething()
//    {
//        return;
//    }
//private:
//};
//
//class BabyGrasshopper : public Actor
//{
//public:
//    BabyGrasshopper(int startX, int startY, StudentWorld* world, Direction dir = random_direction(), int depth = 1,  double size = 0.30, int imageID = IID_BABY_GRASSHOPPER);
//    
//    virtual void doSomething();
// 
//    
//private:
//    int m_hit_points;
//    bool isDead;
//    bool crowd_control;
//    int stun_index;
//};
//
//class AdultGrasshopper : public Actor
//{
//public:
//    AdultGrasshopper(int startX, int startY, StudentWorld* world, Direction dir = random_direction(), int depth = 1,  double size = 0.30, int imageID = IID_ADULT_GRASSHOPPER):Actor(imageID, startX, startY, world, dir, depth, size)
//    {}
//    virtual void doSomething()
//    {
//        return;
//    }
//    
//};
//
//class Anthill : public Actor
//{
//public:
//    Anthill(int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 2,  double size = 0.30, int imageID = IID_ANT_HILL):Actor(imageID, startX, startY, world, dir, depth, size){}
//    virtual void doSomething()
//    {
//        return;
//    }
//};
//
//class Food : public Actor
//{
//public:
//    Food(int startX, int startY, StudentWorld* world, Direction dir = right, int units, int depth = 2,  double size = 0.30, int imageID = IID_FOOD):Actor(imageID, startX, startY, world, dir, depth, size)
//    {
//        
//    }
//    virtual void doSomething()
//    {
//        return;
//    }
//};
//
//class Water : public Actor
//{
//public:
//    Water(int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 1,  double size = 0.30, int imageID = IID_WATER_POOL ):Actor(IID_WATER_POOL, startX, startY, world, dir, depth, size){}
//    virtual void doSomething()
//    {
//        return;
//    }
//
//};
//
//class Poison : public Actor
//{
//public:
//    Poison(int startX, int startY, StudentWorld* world, Direction dir = right, int depth = 1,  double size = 0.30, int imageID = IID_POISON ):Actor(imageID, startX, startY, world, dir, depth, size){}
//    virtual void doSomething()
//    {
//        return;
//    }
//
//};
//






#endif // ACTOR_H_
