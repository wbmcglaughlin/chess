//
// Created by Will McGlaughlin on 22/5/2022.
//

#include "minMax.h"

MoveEval MiniMax(Board *board, int depth, enum MinMax minMax) {
    // Checking if you have reached maximum depth or there are no legal moves
    if (depth == 0 || GetAllMovesCount(board) == 0) {
        float eval = GetBoardScore(board);
        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {move, eval};

        return moveEval;
    }

    // Getting the worst possible case
    float bestEval = -INFINITY;
    if (minMax == Min) {
        bestEval = INFINITY;
    }

    // Allocating memory to get moves
    Move *moves = malloc(sizeof(Move) * MAX_MOVES); // NEED TO FREE
    Move bestMove = moves[0];

    int moveCount = 0;
    GetAllLegalMoves(board, moves, &moveCount);

    for (int i = 0; i < moveCount; i++) {
        Board *newBoard = CopyBoard(board);
        UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);

        Move newBestMove = moves[i];

        enum MinMax nextMinMax = minMax;
        if (nextMinMax == minMax) {
            nextMinMax = Max;
        } else {
            nextMinMax = Min;
        }

        // Dive further into minimax
        MoveEval moveEval = MiniMax(newBoard, depth - 1, nextMinMax);

        if (minMax == Max) {
            if (moveEval.eval > bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }
        } else {
            if (moveEval.eval < bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }
        }

        FreeBoard(newBoard);
    }

    free(moves);

    return (MoveEval) {bestMove, bestEval};
}