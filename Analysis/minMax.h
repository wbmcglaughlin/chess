#ifndef CHESS_MINMAX
#define CHESS_MINMAX

#include "../Game/board.h"
#include "../Game/moves.h"

#define INFINITY 100000

struct MoveEval {
    Move move;
    float eval;
};

enum MinMax {
    Min = 0,
    Max = 1
};

typedef struct MoveEval MoveEval;

MoveEval MiniMax(Board *boardState, int depth, enum MinMax minMax);


#endif
