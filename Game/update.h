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

#include "gameInstance.h"
#include "../Graphics/draw.h"
#include "../Systems/bot.h"


void BoardUpdateLoop(Game *gameInstance, BoardDimensions *boardDimensions,
                     int *getMoves, int *selected, int *pieceHeld);
void PlayerTurnCheck(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld);
void BotTurnCheck(Game *gameInstance, BoardDimensions *boardDimensions, int *movesCount);

#endif //CHESS_UPDATE_H
