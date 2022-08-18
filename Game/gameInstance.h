//
// Created by Will McGlaughlin on 18/8/2022.
//

#ifndef CHESS_GAMEINSTANCE_H
#define CHESS_GAMEINSTANCE_H

#include "board.h"
#include "moves.h"
#include "../Systems/bot.h"

struct Game {
    Board *board;
    int *moveSquares;
    int moveCount;
    int players[2];
    BotInput botInput[2];
};

typedef struct Game Game;

#endif //CHESS_GAMEINSTANCE_H
