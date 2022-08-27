//
// Created by wbmcg on 15/05/2022.
//

#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include <pthread.h>
#include <math.h>

#include "../Game/draw.h"
#include "../Analysis/minMax.h"

struct BotInput {
    Board *board;
    Move *move;
    float *eval;
    int *hasMove;
    int *calls;
    float *calculatedBoardEvaluation;
    int *threadStarted;
    int *threadRunning;
};

typedef struct BotInput BotInput;

#define WORST_SCORE 10000

//#define DEPTHS 6
//static int depthStage[DEPTHS] = {4, 5, 6, 7, 8, 1};
//static int depthPhase[DEPTHS] = {19, 16, 8, 5, 3, 0};

#define DEPTHS 2
static int depthStage[DEPTHS] = {4, 1};
static int depthPhase[DEPTHS] = {24, 0};

BotInput *CreateBotInput(Board *board);
void FreeBotInput(BotInput *botInput);

Move RandomBot(Board *board);
Move CaptureFirstBot(Board *board);
Move BestScoreBot(Board *board);
void* MiniMaxBot(void *botInput);

#endif //CHESS_BOT_H
