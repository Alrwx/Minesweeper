#pragma once
#include <iostream>
#include "Tile.h"
using namespace std;

class Board {
  private:
    int rowCount;
    int colCount;
    int mineCount;
    vector<vector<Tile*>> board;
   public:
     Board(int rowCount, int colCount, int mineCount);
};