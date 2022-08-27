//
// Created by Will McGlaughlin on 27/4/2022.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"

#define BOARD_PERCENT .9
#define SQUARE_COUNT 8
#define SQUARES SQUARE_COUNT * SQUARE_COUNT
#define MAX_FEN_LENGTH 90
#define UNIQUE_PIECE_TEXTURES 12

#define PIECES "PpNnBbRrQqKk"
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

/* board representation */
#define WHITE_PIECE  0
#define BLACK_PIECE  1

#define WHITE_PAWN      (2*PAWN   + WHITE_PIECE)
#define BLACK_PAWN      (2*PAWN   + BLACK_PIECE)
#define WHITE_KNIGHT    (2*KNIGHT + WHITE_PIECE)
#define BLACK_KNIGHT    (2*KNIGHT + BLACK_PIECE)
#define WHITE_BISHOP    (2*BISHOP + WHITE_PIECE)
#define BLACK_BISHOP    (2*BISHOP + BLACK_PIECE)
#define WHITE_ROOK      (2*ROOK   + WHITE_PIECE)
#define BLACK_ROOK      (2*ROOK   + BLACK_PIECE)
#define WHITE_QUEEN     (2*QUEEN  + WHITE_PIECE)
#define BLACK_QUEEN     (2*QUEEN  + BLACK_PIECE)
#define WHITE_KING      (2*KING   + WHITE_PIECE)
#define BLACK_KING      (2*KING   + BLACK_PIECE)
#define EMPTY           (BLACK_KING  +  1)

#define PCOLOR(p) ((p)&1)

enum MoveType {
    INVALID = 0,
    TO_EMPTY = 1,
    CAPTURE = 2,
    ENPASSANT = 3,
    CASTLE = 4,
    PROMOTION = 5
};

enum PlayerType {
    BOT = 0,
    PLAYER = 1
};

struct BoardDimensions {
    int screenWidth;
    int screenHeight;
    int definingLength;
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
    int type;
    int pos;
    int color;
    float score;
};

struct Board {
    struct Piece *Board;
    float eval;
    int turn;
    int movesCount;
    int castle[4];
    int kingPos[2];
    int enpassant;
    int halfMoveClock;
    int moveCount;
    int checkMate;
    int draw;
};

typedef struct BoardSquare BoardSquare;
typedef struct Board Board;
typedef struct Piece Piece;
typedef struct BoardDimensions BoardDimensions;

void BoardUpdateLoop();

void SwapPieces(Board *board, int pos1, int pos2);
void GetSelected(int *selected, float mousePosX, float mousePosY, BoardDimensions *boardDimensions);
int PosIsValid(int pos);

void GetSquare(BoardSquare *boardSquare, int i, int sideSize, int cornerX, int cornerY);
void FenToBoard(const char *fen, Board *board);

Board* CopyBoard(Board *oldBoard);
Board* CreateBoard(void);
void FreeBoard(Board *board);

#endif //CHESS_BOARD_H
