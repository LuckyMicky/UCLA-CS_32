//
//  mazequeue.cpp
//  Homework 2
//
//  Created by SHUDAXUAN on 2/7/17.
//  Copyright © 2017 SHUDAXUAN. All rights reserved.
//

#include <iostream>
#include <queue>
#include <string>

using namespace std;

const char OPEN = '.';
const char TRIED = '#';

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

void explore(string maze[], queue<Coord>& m_cor, int r, int c)
{
    if (maze[r][c] == OPEN)
    {
        m_cor.push(Coord(r,c));
        maze[r][c] = TRIED;
    }
}
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> m_cor;
    explore(maze, m_cor, sr, sc);
    
    while (! m_cor.empty())
    {
        Coord top = m_cor.front();
        m_cor.pop();
        
        const int tr = top.r();
        const int tc = top.c();
        
        if (tr == er && tc == ec)
            return true;
        
        explore(maze, m_cor, tr-1, tc);
        explore(maze, m_cor, tr, tc+1);
        explore(maze, m_cor, tr+1, tc);
        explore(maze, m_cor, tr, tc-1);
    }
    return false;
}

