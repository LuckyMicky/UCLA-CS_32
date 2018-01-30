// History.h

#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);

        // Accessors
    void display() const;

        // Mutators
    bool record(int r, int c);

  private:
    int  m_grid[MAXROWS][MAXCOLS];
    int  m_rows;
    int  m_cols;
};

#endif // HISTORY_INCLUDED
