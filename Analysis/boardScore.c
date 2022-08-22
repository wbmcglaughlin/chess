//
// Created by Will McGlaughlin on 23/8/2022.
//

#include "boardScore.h"

float GetBoardScore(Board *board) {
    float score = 0.0f;
    for (int i = 0; i < SQUARES; i++) {
        score += board->Board[i].score;
    }

    return score;
}