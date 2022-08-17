//
// Created by Will McGlaughlin on 17/8/2022.
//

#ifndef CHESS_MOVELIST_H
#define CHESS_MOVELIST_H

#include "moves.h"

struct MoveNode {
    Move move;
    struct MoveNode* next;
};

typedef struct MoveNode MoveNode;
typedef MoveNode *MoveNodePtr;

void PrintMoveNodeList(MoveNodePtr head);
void InsertMoveNode(MoveNodePtr *head, Move move);
void DeleteMoveNode(MoveNodePtr *head);
int IsMoveNodeListEmpty(MoveNodePtr head);

#endif //CHESS_MOVELIST_H
