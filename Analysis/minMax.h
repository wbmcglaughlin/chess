#ifndef CHESS_MINMAX
#define CHESS_MINMAX

#include "../Game/board.h"
#include "../Game/moves.h"

#define INFINITY 100000

struct BoardState {
    Board *board;
    Move bestMove;
    float eval;
};

struct MoveEval {
    Move move;
    float eval;
};

enum MinMax {
    Min = 0,
    Max = 1
};

typedef struct BoardState BoardState;
typedef struct MoveEval MoveEval;

void GetBoardMoves(BoardState *boardMoves);
MoveEval MiniMax(Board *boardState, int depth, enum MinMax minMax);
BoardState *CreateBoardState();
void FreeBoardState(BoardState *boardState);

#endif
