//
// Created by Will McGlaughlin on 18/8/2022.
//

#ifndef CHESS_GAMEINSTANCE_H
#define CHESS_GAMEINSTANCE_H

#include "board.h"
#include "moves.h"
#include "../Systems/bot.h"

#define MAX_TURNS 5000

struct Game {
    Board *board;
    Move *moves;
    int *moveSquares;
    int movesCount;
    int moveCount;
    int players[2];
    BotInput botInput;
};

typedef struct Game Game;

Game *NewGameInstanceFromFen(char*fen);
void FreeGameInstance(Game *game);

#endif //CHESS_GAMEINSTANCE_H
