//
// Created by Will McGlaughlin on 20/8/2022.
//

#include "arrows.h"

void DrawArrows(MoveNodePtr *firstArrow, int *squarePressed, int *squareReleased, Vector2 *mousePosition, BoardDimensions *boardDimensions) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        GetSelected(squarePressed, mousePosition->x, mousePosition->y, boardDimensions);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!IsMoveNodeListEmpty(*firstArrow)) {
            while ((*firstArrow)->next != NULL) {
                DeleteMoveNode(firstArrow);
            }
            DeleteMoveNode(firstArrow);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        GetSelected(squareReleased, mousePosition->x, mousePosition->y, boardDimensions);

        if (*squarePressed != -1 && *squareReleased != -1) {
            Move arrowMove = (Move) {*squarePressed, *squareReleased, 0};
            InsertMoveNode(firstArrow, arrowMove);
        }

        *squarePressed = -1;
        *squareReleased = -1;
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        *squarePressed = -1;
    }

    MoveNodePtr startPtr = *firstArrow;
    if (!IsMoveNodeListEmpty(*firstArrow)) {
        DrawArrow(startPtr->move.pos, startPtr->move.target, BLUE, boardDimensions);
        while (startPtr->next != NULL) {
            startPtr = startPtr->next;
            DrawArrow(startPtr->move.pos, startPtr->move.target, BLUE, boardDimensions);
        }
    }
}
