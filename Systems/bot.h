//
// Created by wbmcg on 15/05/2022.
//

#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include "../Game/board.h"
#include "../Game/moves.h"
#include "math.h"
#include "../Analysis/minMax.h"

#define WORST_SCORE 10000

Move RandomBot(Board *board);
Move CaptureFirstBot(Board *board);
Move BestScoreBot(Board *board);
MoveEval MiniMaxBot(Board *board, int depth);

#endif //CHESS_BOT_H
