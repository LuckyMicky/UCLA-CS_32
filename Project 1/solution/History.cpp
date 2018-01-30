// History.cpp

#include "History.h"
#include "globals.h"
#include <iostream>
#include <cstdlib>
using namespace std;

History::History(int nRows, int nCols)
 : m_rows(nRows), m_cols(nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** History created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    for (int r = 0; r < m_rows; r++)
       for (int c = 0; c < m_cols; c++)
            m_grid[r][c] = 0;
}

void History::display() const
{
    clearScreen();

      // Draw the grid
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            char ch = '.';
            int n = m_grid[r][c];
            if (n >= 26)
                ch = 'Z';
            else if (n > 0)
                ch = 'A' + n-1;
            cout << ch;
        }
        cout << endl;
    }
    cout << endl;
}

bool History::record(int r, int c)
{
    if (r <= 0  ||  c <= 0  ||  r > m_rows  ||  c > m_cols)
        return false;
    m_grid[r-1][c-1]++;
    return true;
}
