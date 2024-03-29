#ifndef CHESS_MINMAX
#define CHESS_MINMAX

#include "../Game/moves.h"
#include "../Analysis/boardScore.h"

#define BOARD_INFINITY 100000

struct MoveEval {
    Move *move;
    float eval;
};

enum MinMax {
    Min = BLACK_PIECE,
    Max = WHITE_PIECE
};

typedef struct MoveEval MoveEval;

MoveEval MiniMaxAB(Board *board, int depth, enum MinMax minMax, float *alpha, float *beta, int *calls);
MoveEval MiniMax(Board *boardState, int depth, enum MinMax minMax, int *calls);
MoveEval * GetEmptyMoveEval();

#endif
