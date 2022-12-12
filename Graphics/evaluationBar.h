//
// Created by Will McGlaughlin on 28/8/2022.
//

#ifndef CHESS_EVALUATIONBAR_H
#define CHESS_EVALUATIONBAR_H

#include "math.h"

#include "raylib.h"

/// Draws the evaluation bar given a certain rectangle and pointer to evaluation bar.
///
/// Evaluation is clamped using atan, between a value a [-1 and 1], and then extended to
/// the rectangles height.
void DrawEvaluationBar(Rectangle *rectangle, const float *evaluation);

#endif //CHESS_EVALUATIONBAR_H
