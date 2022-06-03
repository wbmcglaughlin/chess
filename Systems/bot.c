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
        return moves[rand() % movesCount];
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