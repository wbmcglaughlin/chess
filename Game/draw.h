//
// Created by Will McGlaughlin on 13/5/2022.
//

#ifndef CHESS_DRAW_H
#define CHESS_DRAW_H

#include "raylib.h"
#include "board.h"

void DrawBoard(BoardDimensions *boardDimensions, int *moves, int selected);
void DrawPieces(BoardDimensions *boardDimensions, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition);
void DrawBoardInfo(Board *board, BoardDimensions *boardDimensions);

#endif //CHESS_DRAW_H
