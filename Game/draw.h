//
// Created by Will McGlaughlin on 13/5/2022.
//

#ifndef CHESS_DRAW_H
#define CHESS_DRAW_H

#include <math.h>

#include "raylib.h"

#include "board.h"
#include "moves.h"


#define FONT_SIZE 20

void DrawBoard(BoardDimensions *boardDimensions, const int *moves, int selected);
void DrawPieces(BoardDimensions *boardDimensions, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition);
void DrawSquareValues(BoardDimensions *boardDimensions, Board *board);
void ListLegalMoves(Move *moves, int movesCount, BoardDimensions *boardDimensions);
void DrawArrow(int startSquare, int endSquare, Color color, BoardDimensions *bd);

#endif //CHESS_DRAW_H
