//
// Created by wbmcg on 15/05/2022.
//

#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include "../Game/board.h"
#include "../Game/moves.h"
#include "math.h"
#include "../Analysis/minMax.h"

#include <pthread.h>

#define WORST_SCORE 10000

struct BotInput {
    Board *board;
    MoveEval *moveEval;
    int *hasMove;
    Move *move;
};

typedef struct BotInput BotInput;

Move RandomBot(Board *board);
Move CaptureFirstBot(Board *board);
Move BestScoreBot(Board *board);
void* MiniMaxBot(void *botInput);

#endif //CHESS_BOT_H
