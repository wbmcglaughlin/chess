//
// Created by Will McGlaughlin on 20/8/2022.
//

#ifndef CHESS_ARROWS_H
#define CHESS_ARROWS_H

#include "raylib.h"

#include "moveList.h"
#include "draw.h"

void DrawArrows(MoveNodePtr *firstArrow, int *squarePressed, int *squareReleased, Vector2 *mousePosition, BoardDimensions *boardDimensions);

#endif //CHESS_ARROWS_H
