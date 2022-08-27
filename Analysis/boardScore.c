//
// Created by Will McGlaughlin on 23/8/2022.
//

#include "boardScore.h"

float GetBoardScore(Board *board) {
    float score = 0.0f;
    for (int i = 0; i < SQUARES; i++) {
        score += board->Board[i].score;
    }

    float centiPawn = 75.0f;
    float positionalEval = (float) PiecePositionEval(board);

    score += positionalEval / centiPawn;

    return score;
}