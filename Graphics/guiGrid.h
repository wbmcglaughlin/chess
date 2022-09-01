//
// Created by Will McGlaughlin on 28/8/2022.
//

#ifndef CHESS_GUIGRID_H
#define CHESS_GUIGRID_H

#include "raylib.h"
#include "stdlib.h"

struct Grid {
    Rectangle *recs;
    Rectangle *rowRecs;
    int rows;
    int cols;
};

typedef struct Grid Grid;

Grid *SetGridRectangles(float cornerX, float cornerY, float width, float height, int rows, int cols, float border);
void DrawGridRectangle(Grid *grid, int rect, Color color);

#endif //CHESS_GUIGRID_H
