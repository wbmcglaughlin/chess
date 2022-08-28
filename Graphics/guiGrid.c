//
// Created by Will McGlaughlin on 28/8/2022.
//

#include "guiGrid.h"

Grid *SetGridRectangles(float cornerX, float cornerY, float width, float height, int rows, int cols) {
    Grid *grid = malloc(sizeof (Grid));
    if (grid == NULL) {
        exit(-1);
    }

    grid->recs = malloc(sizeof (Rectangle) * rows * cols);
    if (grid->recs == NULL) {
        exit(-1);
    }

    grid->rows = rows;
    grid->cols = cols;

    float sideWidth = width / (float) cols;
    float sideHeight = height / (float) rows;

    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            grid->recs[r * cols + c] = (Rectangle) {cornerX + (float) c * sideWidth,
                                                   cornerY + (float) r * sideHeight,
                                                   sideWidth,
                                                   sideHeight};
        }
    }

    return grid;
}

void DrawGridRectangle(Grid *grid, int rect, Color color) {
    DrawRectangleRec(grid->recs[rect], color);
}
