//
// Created by Will McGlaughlin on 22/5/2022.
//

#include "minMax.h"

MoveEval MiniMaxAB(Board *board, int depth, enum MinMax minMax, float *alpha, float *beta, int *calls) {
    (*calls) += 1;
    // Checking if you have reached maximum depth or there are no legal moves
    if (depth == 0) {
        float eval = GetBoardScore(board);
        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, eval};

        return moveEval;
    }
    if (board->checkMate) {
        float eval = -BOARD_INFINITY;

        if (minMax == Min) {
            eval = BOARD_INFINITY;
        }

        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, eval};

        return moveEval;
    }

    if (board->draw) {
        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, 0.0f};

        return moveEval;
    }

    // Getting the worst possible case
    float bestEval = -BOARD_INFINITY;
    if (minMax == Min) {
        bestEval = BOARD_INFINITY;
    }

    // Allocating memory to get moves
    Move *moves = malloc(sizeof(Move) * MAX_MOVES); // NEED TO FREE
    Move bestMove = moves[0];

    int moveCount = 0;
    GetAllLegalMovesSorted(board, moves, &moveCount);

    float alpha_new = *alpha;
    float beta_new = *beta;

    for (int i = 0; i < moveCount; i++) {
        Board *newBoard = CopyBoard(board);
        UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);
        CheckStatus(newBoard);
        Move newBestMove = moves[i];

        enum MinMax nextMinMax = minMax;
        if (nextMinMax == Min) {
            nextMinMax = Max;
        } else if (nextMinMax == Max) {
            nextMinMax = Min;
        }

        // Dive further into minimax
        MoveEval moveEval = MiniMaxAB(newBoard, depth - 1, nextMinMax, &alpha_new, &beta_new, calls);

        if (minMax == Max) {
            if (moveEval.eval > bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }

            if ((alpha_new) < bestEval) {
                alpha_new = bestEval;
            }

            if (beta_new <= bestEval) {
                FreeBoard(newBoard);
                free(moves);
                return (MoveEval) {&bestMove, bestEval};
            }
        } else if (minMax == Min) {
            if (moveEval.eval < bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }

            if (beta_new > bestEval) {
                beta_new = bestEval;
            }

            if (bestEval <= alpha_new) {
                FreeBoard(newBoard);
                free(moves);
                return (MoveEval) {&bestMove, bestEval};
            }
        }

        FreeBoard(newBoard);
    }

    free(moves);

    return (MoveEval) {&bestMove, bestEval};
}

MoveEval MiniMax(Board *board, int depth, enum MinMax minMax, int *calls) {
    (*calls) += 1;
    // Checking if you have reached maximum depth or there are no legal moves
    if (depth == 0) {
        float eval = GetBoardScore(board);
        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, eval};

        return moveEval;
    }
    if (board->checkMate) {
        float eval = BOARD_INFINITY;

        if (minMax == Min) {
            eval = -BOARD_INFINITY;
        }

        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, eval};

        return moveEval;
    }

    if (board->draw) {
        Move move = (Move) {0, 0, 0};
        MoveEval moveEval = (MoveEval) {&move, 0.0f};

        return moveEval;
    }

    // Getting the worst possible case
    float bestEval = -BOARD_INFINITY;
    if (minMax == Min) {
        bestEval = BOARD_INFINITY;
    }

    // Allocating memory to get moves
    Move *moves = malloc(sizeof(Move) * MAX_MOVES); // NEED TO FREE
    Move bestMove = moves[0];

    int moveCount = 0;
    GetAllLegalMoves(board, moves, &moveCount);

    for (int i = 0; i < moveCount; i++) {
        Board *newBoard = CopyBoard(board);
        UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);
        CheckStatus(board);
        Move newBestMove = moves[i];

        enum MinMax nextMinMax = minMax;
        if (nextMinMax == Min) {
            nextMinMax = Max;
        } else if (nextMinMax == Max) {
            nextMinMax = Min;
        }

        // Dive further into minimax
        MoveEval moveEval = MiniMax(newBoard, depth - 1, nextMinMax, calls);

        if (minMax == Max) {
            if (moveEval.eval > bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }
        } else if (minMax == Min){
            if (moveEval.eval < bestEval) {
                bestEval = moveEval.eval;
                bestMove = newBestMove;
            }
        }

        FreeBoard(newBoard);
    }

    free(moves);

    return (MoveEval) {&bestMove, bestEval};
}

MoveEval * GetEmptyMoveEval() {
    MoveEval *moveEval = malloc(sizeof (MoveEval));
    Move *move = malloc(sizeof (Move));
    moveEval->move = move;
    moveEval->eval = 0.0f;
    return moveEval;
}