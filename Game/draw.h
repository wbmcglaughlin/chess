//
// Created by Will McGlaughlin on 13/5/2022.
//

#ifndef CHESS_DRAW_H
#define CHESS_DRAW_H

#include "raylib.h"
#include "board.h"

void DrawBoard(int cornerX, int cornerY, int sideSize, int *moves, int selected);
void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition);

#endif //CHESS_DRAW_H
