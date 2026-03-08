#pragma once
#include <vector>

struct Grid {
public:
    Grid();
    void DrawGrid();
    int cellsArray[20][30] = {};
private:
    int rows;
    int columns;
    int pixels;
};


