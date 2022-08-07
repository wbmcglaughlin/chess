//
// Created by Will McGlaughlin on 22/5/2022.
//

#include "minMax.h"

MoveEval MiniMax(BoardState *boardState, int depth, int minMax) {
    if (depth == 0 || GetAllMovesCount(boardState->board) == 0) {
        return (MoveEval) {boardState->bestMove, boardState->eval};
    }

    if (minMax == 0) {
        float maxEval = -INFINITY;
        Move *bestMove = NULL;

        Move *moves = malloc(sizeof(Move) * MAX_MOVES);
        int moveCount = 0;
        GetAllLegalMoves(boardState->board, moves, &moveCount);

        for (int i = 0; i < moveCount; i++) {
            Board *newBoard = CopyBoard(boardState->board);
            UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);

            BoardState *boardStateNew = malloc(sizeof (BoardState) + sizeof (Board) + sizeof (Move));
            boardStateNew->board = newBoard;
            boardState->bestMove = &moves[i];
            boardState->eval = 0;

            MoveEval moveEval = MiniMax(boardStateNew, depth - 1, 0);
            if (moveEval.eval > maxEval) {
                maxEval = moveEval.eval;
                bestMove = &moves[i];
            }
            free(newBoard);
            free(boardStateNew);
        }

        free(moves);

        return (MoveEval) {bestMove, maxEval};

    } else if (minMax == 1) {
        float minEval = INFINITY;
        Move *bestMove = NULL;

        Move *moves = malloc(sizeof(Move) * MAX_MOVES);
        int moveCount = 0;
        GetAllLegalMoves(boardState->board, moves, &moveCount);

        for (int i = 0; i < moveCount; i++) {
            Board *newBoard = CopyBoard(boardState->board);
            UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);

            BoardState *boardStateNew = malloc(sizeof (BoardState) + sizeof (Board) + sizeof (Move));
            boardStateNew->board = newBoard;
            boardState->bestMove = &moves[i];
            boardState->eval = 0;

            MoveEval moveEval = MiniMax(boardStateNew, depth - 1, 1);
            if (moveEval.eval < minEval) {
                minEval = moveEval.eval;
                bestMove = &moves[i];
            }
            free(newBoard);
            free(boardStateNew);
        }

        free(moves);

        return (MoveEval) {bestMove, minEval};
    }
}