//
// Created by Will McGlaughlin on 23/8/2022.
//

#include "boardScore.h"

float GetBoardScore(Board *board) {
    float score = 0.0f;
    for (int i = 0; i < SQUARES; i++) {
        score += board->Board[i].score;
    }

    float positionalScore = (float) PiecePositionEval(board) / 100.0f;

    score += -(2.0f * (board->turn) - 1) * positionalScore;

    return score;
}