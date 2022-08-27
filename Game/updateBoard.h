//
// Created by Will McGlaughlin on 27/8/2022.
//

#ifndef CHESS_UPDATEBOARD_H
#define CHESS_UPDATEBOARD_H

#include "board.h"
#include "moves.h"

void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType);
void CheckStatus(Board *board);

#endif //CHESS_UPDATEBOARD_H
