#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include <map>
#include <set>


using namespace std;


class Actor;

class Compiler;


typedef set<Actor*> square;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
    {}

    virtual int init();
    
    virtual int move();

    virtual void cleanUp();
    
    // Can an insect move to x,y?
    bool canMoveTo(int x, int y) const;
    
    // Add an actor to the world
    void addActor(Actor* a);
    
    // If an item that can be picked up to be eaten is at x,y, return a
    // pointer to it; otherwise, return a null pointer.  (Edible items are
    // only ever going be food.)
    Actor* getEdibleAt(int x, int y) const;
    
    // If a pheromone of the indicated colony is at x,y, return a pointer
    // to it; otherwise, return a null pointer.
    Actor* getPheromoneAt(int x, int y, int colony) const;
    
    // Is an enemy of an ant of the indicated colony at x,y?
    bool isEnemyAt(int x, int y, int colony) const;
    
    // Is something dangerous to an ant of the indicated colony at x,y?
    bool isDangerAt(int x, int y, int colony) const;
    
    // Is the anthill of the indicated colony at x,y?
    bool isAntHillAt(int x, int y, int colony) const;
    
    // Bite an enemy of an ant of the indicated colony at me's location
    // (other than me; insects don't bite themselves).  Return true if an
    // enemy was bitten.
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    
    // Poison all poisonable actors at x,y.
    bool poisonAllPoisonableAt(int x, int y);
    
    // Stun all stunnable actors at x,y.
    bool stunAllStunnableAt(int x, int y);
    
    // Record another ant birth for the indicated colony.
    void increaseScore(int colony);
    
    void updateTickCount();
    
    void updateDispayText();
    
    void getfieldcorr(int &x, int &y, Field::FieldItem a);
    
    void insertNewActor(int m, int n, Actor* newActor);
    
    int getCurrentTicks()
    {
        return m_tick;
    }
    
    string Format(int ticks);//, int antsAnt0, int antsAnt1, int antsAnt2, int antsAnt3, int winningAntNumber);
    
    

    
private:
    int m_tick;
    square m_map[VIEW_HEIGHT][VIEW_WIDTH];

    
};

//StudentWorld m_world("/Users/shudaxuan/Desktop/Bugs/");

#endif // STUDENTWORLD_H_
