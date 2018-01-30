#include "StudentWorld.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
    Field f;
    string fieldFile = getFieldFilename();
    string error;
    if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
        {
        setError(fieldFile + " " + error);
        return GWSTATUS_LEVEL_ERROR;    // something bad happened!
        }
//    m_field = &f;
    
    Compiler *compilerForEntrant[4];
    AntHill *ah[4];
    vector<string> fileNames = getFilenamesOfAntPrograms();
    
    for (int i = 0; i < fileNames.size(); i++)
    {
        compilerForEntrant[i] = new Compiler;
        
        if (! compilerForEntrant[i] -> compile(fileNames[i], error))
        {
            setError(fileNames[i] + " " + error);
        }
        
//        int x,y;
//        Field::FieldItem cur;
//        
//        if (i == 0)
//            cur = Field::anthill0;
//        if (i == 1)
//            cur = Field::anthill1;
//        if (i == 2)
//            cur = Field::anthill2;
//        if (i == 3)
//            cur = Field::anthill3;
        
//        getfieldcorr(x, y, cur);
        
//        ah[i] = new AntHill(this, y, x, i , compilerForEntrant[i]);
//        addActor(ah[i]);

    }
    
    for (int i = 0; i <VIEW_WIDTH; i++)
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
           
            Field::FieldItem item = f.getContentsOf(i, j);
            switch (item) {
                case Field::empty:
                    break;
                case Field::anthill0:
                {
                    ah[0] = new AntHill(this, i, j, 0 , compilerForEntrant[i]);
                    addActor(ah[0]);
                    break;
                }
                case Field::FieldItem::anthill1:
                {
                    ah[1] = new AntHill(this, i, j, 1 , compilerForEntrant[i]);
                    addActor(ah[1]);
                    break;
                }
                case Field::anthill2:
                {
                    ah[2] = new AntHill(this, i, j, 2 , compilerForEntrant[i]);
                    addActor(ah[2]);
                    break;
                }
                case Field::anthill3:
                {
                    ah[3] = new AntHill(this, i, j, 3 , compilerForEntrant[i]);
                    addActor(ah[3]);
                    break;
                }
                case Field::food:
                {
                    Food* food = new Food(this, i, j, 6000);
                    addActor(food);
                    break;
                }

                case Field::grasshopper:
                {
                    BabyGrasshopper* g = new BabyGrasshopper(this, i, j);
                    addActor(g);
                    break;
                }

                case Field::water:
                {
                    WaterPool* w = new WaterPool(this, i, j);
                    addActor(w);
                    break;
                }
                case Field::rock:
                {
                    Pebble *rock = new Pebble(this, i, j);
                    addActor(rock);
                    break;
                }
                case Field::poison:
                {
                    Poison* p = new Poison(this, i, j);
                    addActor(p);
                    break;
                }

                    
            }
        }
    
    m_tick = 0;
   
    updateTickCount();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
     updateTickCount();
    
    for (int i = 0; i <VIEW_WIDTH; i++)
        for (int j = 0; j < VIEW_HEIGHT; j++)
        {
            //square cur_square = m_map[j][i];
            if (m_map[j][i].size() != 0)
            {
                for (square::iterator it = m_map[j][i].begin(); it != m_map[j][i].end();it++)
                {
//                    int OldX = j;
//                    int OldY = i;
                    if (!(*it)->isDead())
                        (*it)->doSomething();
                    //if (q has moved from its old square to a new square)
                    // updateTheDataStructureThatTracksWhereEachActorsIs(q,oldX,oldY);
//                    if ((*it) -> getX() != j || (*it) -> getY() != i)
//                    {
//                        m_map[(*it) -> getY()][(*it) -> getX()].insert(*it);
//                        m_map[j][i].erase(it);
//                    }
                }
            }
            
        }
//    for (int i = 0; i <VIEW_WIDTH; i++)
//        for (int j = 0; j < VIEW_HEIGHT; j++)
//        {
//            if (m_map[j][i].size() != 0)
//            {
//                for (square::iterator it = m_map[j][i].begin(); it != m_map[j][i].end();++it)
//                {
//                    if ((*it)->isDead())
//                    {
//                        m_map[j][i].erase(it);
//                        delete *it;
//                    }
//                    
//                }
//            }
//        }
    
    updateDispayText();
    
    
    
    if(m_tick == 2000)
    {
        return GWSTATUS_NO_WINNER;
    }
    else
        return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i <VIEW_WIDTH; i++)
        for (int j = 0; j < VIEW_HEIGHT; j++)
            if (m_map[j][i].size() != 0)
            {
                square::iterator it = m_map[j][i].begin();
                while (it != m_map[j][i].end()) {
                    delete (*it);
                    it = m_map[j][i].erase(it);
                }
            }
    return;
}

void StudentWorld::updateTickCount()
{
    m_tick++;
}


void StudentWorld::updateDispayText()
{
    int ticks = getCurrentTicks();
//    int antsAnt0, antsAnt1, antsAnt2, antsAnt3;
//    int winningAntNumber;
//    vector<string> name = getFilenamesOfAntPrograms();
//    antsAnt0 = getNumberofAntsForAnt(0);
//    antsAnt0 = getNumberofAntsForAnt(1);
//    antsAnt0 = getNumberofAntsForAnt(2);
//    antsAnt0 = getNumberofAntsForAnt(3);
    
//    winningAntNumber = getWinningAntNumber();
    

    
    string s = Format(ticks); //, antsAnt0, antsAnt1, antsAnt2, antsAnt3, winningAntNumber);
    setGameStatText(s);
}

string StudentWorld::Format(int ticks) //, int antsAnt0, int antsAnt1, int antsAnt2, int antsAnt3, int winningAntNumber)
{
    ostringstream oss;
    oss.setf(ios::fixed);
    oss<< "Ticks: " << ticks << " - ";
    return oss.str();
}

void StudentWorld::insertNewActor(int m, int n, Actor* newActor)
{
    m_map[n][m].insert(newActor);
}

bool StudentWorld::canMoveTo(int x,int y) const
{
    return true;
}

// Add an actor to the world
void StudentWorld::addActor(Actor *a)
{
    int x = a->getX();
    int y = a->getY();
    m_map[y][x].insert(a);
}

// If an item that can be picked up to be eaten is at x,y, return a
// pointer to it; otherwise, return a null pointer.  (Edible items are
// only ever going be food.)
Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    square cur = m_map[y][x];
    for (square::iterator it = cur.begin(); it != cur.end(); it++)
    {
        if((*it)->isEdible())
            return (*it);
    }
    return nullptr;
}

// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
//Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
//{
//    return;
//}

// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    return true;
}

// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    return true;
}

// Is the anthill of the indicated colony at x,y?
bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    return true;
}

// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    return true;
}

// Poison all poisonable actors at x,y.
bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    return true;
}

// Stun all stunnable actors at x,y.
bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    return true;
}

// Record another ant birth for the indicated colony.
void StudentWorld::increaseScore(int colony)
{
    
}

//void StudentWorld::getfieldcorr(int &x, int &y, Field::FieldItem a)
//{
//    for (int i = 0; i <VIEW_WIDTH; i++)
//    {
//        for (int j = 0; j < VIEW_HEIGHT; j++)
//        {
//            Field::FieldItem item = m_field->getContentsOf(i, j);
//            if (item == a)
//            {
//                x = j;
//                y = i;
//                goto stop;
//            }
//        }
//        
//    }
//stop:
//    return;
//}









