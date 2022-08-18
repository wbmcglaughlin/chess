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
#include <pthread.h>

#include "moves.h"
#include "board.h"
#include "draw.h"
#include "../Systems/bot.h"

#define DEPTH_SEARCH 1

void BoardUpdateLoop(int players[2], Board *board, BoardDimensions *boardDimensions, BotInput *botInput, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld);
void PlayerTurnCheck(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld);
void BotTurnCheck(BotInput *botInput, BoardDimensions *boardDimensions, int *movesCount);

#endif //CHESS_UPDATE_H
