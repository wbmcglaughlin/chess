//
// Created by wbmcg on 15/05/2022.
//

#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include "../Game/board.h"
#include "../Game/moves.h"
#include "math.h"
#include "../Analysis/minMax.h"
#include "../Game/draw.h"

#include <pthread.h>

#define WORST_SCORE 10000

struct BotInput {
    Board *board;
    Move *move;
    float *eval;
    int *hasMove;
    int *calls;
    int *threadStarted;
};

typedef struct BotInput BotInput;

BotInput *CreateBotInput(Board *board);
void FreeBotInput(BotInput *botInput);

Move RandomBot(Board *board);
Move CaptureFirstBot(Board *board);
Move BestScoreBot(Board *board);
void* MiniMaxBot(void *botInput);

#endif //CHESS_BOT_H
