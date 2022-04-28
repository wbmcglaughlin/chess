//
// Created by Will McGlaughlin on 27/4/2022.
//

#ifndef EXAMPLE_BOARD_H
#define EXAMPLE_BOARD_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"

#define SQUARE_COUNT 8
#define MAX_FEN_LENGTH 90
#define UNIQUE_PIECE_TEXTURES 12
#define UNIQUE_PIECE_TYPES 6

#define PIECES "pPrRnNbBkKqQ"

struct Piece {
    char type;
    int pos;
    int color;
};

struct Board {
    struct Piece *Board;
    int turn;
    int whiteCastle;
    int blackCastle;
};

typedef struct PieceTextures PieceTextures;
typedef struct Board Board;
typedef struct Piece Piece;

void DrawBoard(int cornerX, int cornerY, int sideSize);
void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12]);
void FenToBoard(char *fen, Board *board);

#endif //EXAMPLE_BOARD_H
