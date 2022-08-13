//
// Created by Will McGlaughlin on 30/5/2022.
//

#ifndef CHESS_UPDATE_H
#define CHESS_UPDATE_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "moves.h"
#include "board.h"
#include "../Systems/bot.h"

#define DEPTH_SEARCH 1

void BoardUpdateLoop(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld);
void PlayerTurnCheck(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld);

#endif //CHESS_UPDATE_H
