#ifndef CHESS_MINMAX
#define CHESS_MINMAX

#include "../Game/board.h"
#include "../Game/moves.h"

struct BoardMoves {
    Board *board;
    Move *moves;
};

typedef struct BoardMoves BoardMoves;

void GetBoardMoves(BoardMoves *boardMoves);

#endif
