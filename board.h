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

#define BOARD_PERCENT .8
#define SQUARE_COUNT 8
#define SQUARES SQUARE_COUNT * SQUARE_COUNT
#define MAX_FEN_LENGTH 90
#define UNIQUE_PIECE_TEXTURES 12
#define UNIQUE_PIECE_TYPES 6

#define PIECES "PpRrNnBbQqKk"

enum MoveType {
    INVALID = 0,
    TO_EMPTY = 1,
    CAPTURE = 2,
    ENPASSANT = 3
};

struct BoardSquare {
    int x;
    int y;
    int cornerX;
    int cornerY;
    int sideSize;
};

struct Piece {
    char type;
    int pos;
    int color;
};

struct Board {
    struct Piece *Board;
    int turn;
    int castle[4];
    int enpassant;
};

typedef struct BoardSquare BoardSquare;
typedef struct Board Board;
typedef struct Piece Piece;

void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType);
void GetSelected(int *selected, float mousePosX, float mousePosY, int cornerX, int cornerY, int sideSize);
int PosIsValid(int pos);

void GetMoves(Board *board, int *moves, int selected);
void ClearMoves(int *moves);
void GetPawnMoves(Board *board, int *moves, int selected);
void GetRookMoves(Board *board, int *moves, int selected);
void GetBishopMoves(Board *board, int *moves, int selected);
void GetKingMoves(Board *board, int *moves, int selected);

void GetSquare(BoardSquare *boardSquare, int i, int sideSize, int cornerX, int cornerY);
void DrawBoard(int cornerX, int cornerY, int sideSize, int *moves, int selected);
void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition);
void FenToBoard(const char *fen, Board *board);

#endif //EXAMPLE_BOARD_H
