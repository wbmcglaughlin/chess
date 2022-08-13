//
// Created by wbmcg on 15/05/2022.
//

#include "bot.h"

Move RandomBot(Board *board) {
    // Makes a random move out of all possible moves
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);
    Move move = moves[rand() % movesCount];
    free(moves);

    return move;
}

Move CaptureFirstBot(Board *board) {
    // Prefers captures over other moves
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    Move move;
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);

    int captureCount = 0;
    for (int i = 0; i < movesCount; i++) {
        if (moves[i].moveType == CAPTURE) {
            captureCount++;
        }
    }
    if (captureCount == 0) {
        Move ret = moves[rand() % movesCount];
        free(moves);
        return ret;
    }

    int captureInd = 0;
    int captureWant = rand() % captureCount;
    for (int i = 0; i < movesCount; i++) {
        if (moves[i].moveType == CAPTURE) {
            if (captureInd == captureWant) {
                move = moves[i];
                free(moves);
                return move;
            }
            captureInd++;
        }
    }
    free(moves);
}

Move BestScoreBot(Board *board) {
    // Makes every move and finds the board with the best score
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    Move move;
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);

    float bestScore = -WORST_SCORE;
    if (board->turn == 0) {
        bestScore = WORST_SCORE;
    }

    for (int i = 0; i < movesCount; i++) {
        Board *newBoard = CopyBoard(board);
        UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);
        float score = GetBoardScore(newBoard);

        if (board->turn == 1) {
            if (score >= bestScore) {
                move = moves[i];
                bestScore = score;
            }
        } else {
            if (score <= bestScore) {
                move = moves[i];
                bestScore = score;
            }
        }
    }

    return move;
}

MoveEval MiniMaxBot(Board *board) {
    MoveEval moveEval = MiniMax(board, 3, Min);

    return moveEval;
}