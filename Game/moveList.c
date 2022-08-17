//
// Created by Will McGlaughlin on 17/8/2022.
//

#include "moveList.h"

void PrintMoveNodeList(MoveNodePtr head) {
    while (head != NULL) {
        printf("(%i, %i, %i) ->", head->move.pos, head->move.target, head->move.moveType);
        head = head->next;
    }
}

void InsertMoveNode(MoveNodePtr *head, Move move) {
    MoveNodePtr newPtr = malloc(sizeof (MoveNode));

    if (newPtr != NULL) {
        newPtr->move = move;
        newPtr->next = *head;

        *head = newPtr;
    }
}

void DeleteMoveNode(MoveNodePtr *head) {
    MoveNodePtr tempPtr = *head;
    *head = (*head)->next;
    free(tempPtr);
}

int IsMoveNodeListEmpty(MoveNodePtr head) {
    return head == NULL;
}

