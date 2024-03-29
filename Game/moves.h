//
// Created by Will McGlaughlin on 13/5/2022.
//

#ifndef CHESS_MOVES_H
#define CHESS_MOVES_H

#include "updateBoard.h"
#include "../Systems/arrayFunc.h"

#define MAX_MOVES 218

struct Move {
    int pos;
    int target;
    enum MoveType moveType;
};

typedef struct Move Move;

void GetMoves(Board *board, int *moves, int selected);
void ClearMoves(int *moves);
void UpdateMovesLegality(Board *board, int *moves, int selected);
int IsMoveLegal(Board *board, int selected, int move, int moveType);
int IsKingInCheck(Board *board, int color);

void GetAllLegalMoves(Board *board, Move *moves, int *movesCount);
int MoveSortFunc(const void *f, const void *s);
void GetAllLegalMovesSorted(Board *board, Move *moves, int *movesCount);
int GetAllLegalMovesCount(Board *board);

void GetPawnMoves(Board *board, int *moves, int selected);
void GetRookMoves(Board *board, int *moves, int selected, int findLegal);
void GetBishopMoves(Board *board, int *moves, int selected);
void GetKingMoves(Board *board, int *moves, int selected);
void GetKnightMoves(Board *board, int *moves, int selected);

#endif //CHESS_MOVES_H
