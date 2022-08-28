//
// Created by Will McGlaughlin on 28/8/2022.
//

#include "evaluationBar.h"

void DrawEvaluationBar(Rectangle *rectangle, const float *evaluation) {
    float clamped_score = atanf(*evaluation) / PI;

    DrawRectangleRec(*rectangle, BLACK);
    DrawRectangle(rectangle->x, rectangle->y, rectangle->width, rectangle->height / 2.0f + rectangle->height / 2.0f * clamped_score, RAYWHITE);

    DrawLine(rectangle->x, rectangle->y + rectangle->height / 2.0f,
             rectangle->x + rectangle->width, rectangle->y + rectangle->height / 2.0f,
             RAYWHITE);
}
