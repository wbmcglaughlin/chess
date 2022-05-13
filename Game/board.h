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
    ENPASSANT = 3,
    CASTLE = 4,
    PROMOTION
};

enum PlayerType {
    BOT = 0,
    PLAYER = 1
};

struct BoardDimensions {
    int cornerX;
    int cornerY;
    int sideSize;
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
    int kingPos[2];
    int kingInCheck[2];
    int enpassant;
    int halfMoveClock;
    int moveCount;
};

typedef struct BoardSquare BoardSquare;
typedef struct Board Board;
typedef struct Piece Piece;
typedef struct BoardDimensions BoardDimensions;

void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType);
void SwapPieces(Board *board, int pos1, int pos2);
void GetSelected(int *selected, float mousePosX, float mousePosY, BoardDimensions *boardDimensions);
int PosIsValid(int pos);
void CopyBoard(Board *newBoard, Board *oldBoard);
void GetSquare(BoardSquare *boardSquare, int i, int sideSize, int cornerX, int cornerY);
void FenToBoard(const char *fen, Board *board);

#endif //EXAMPLE_BOARD_H
