//
// Created by Will McGlaughlin on 27/4/2022.
//

#include "board.h"

void GetSquare(BoardSquare *boardSquare, int i, int squareSize, int cornerX, int cornerY) {
    int x = i % SQUARE_COUNT;
    int y = (int) (i / SQUARE_COUNT);

    boardSquare->x = x;
    boardSquare->y = y;
    boardSquare->sideSize = squareSize;
    boardSquare->cornerX  = cornerX + x * squareSize;
    boardSquare->cornerY = cornerY + (SQUARE_COUNT - y - 1) * squareSize;
}

void GetSelected(int *selected, float mousePosX, float mousePosY, BoardDimensions *boardDimensions) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;

    // mousePosX = cornerX + x * squareWidth
    // mousePosY = cornerY + (SQUARE_COUNT - y - 1) * squareWidth;
    int x = (int) ((mousePosX - (float) boardDimensions->cornerX) / (float) squareWidth);
    int y = SQUARE_COUNT - (int) ((mousePosY - (float) boardDimensions->cornerY) / (float) squareWidth) - 1;

    if (x >= 0 && x < SQUARE_COUNT && y >= 0 && y < SQUARE_COUNT) {
        *selected = x + SQUARE_COUNT * y;
    }
}

int PosIsValid(int pos) {
    if (pos < SQUARES && pos >= 0) {
        return 1;
    }
    return 0;
}

void SwapPieces(Board *board, int pos1, int pos2) {
    Piece *tempPiece = malloc(sizeof (Piece));
    *tempPiece = board->Board[pos1];
    board->Board[pos1] = board->Board[pos2];
    board->Board[pos2] = *tempPiece;
    free(tempPiece);
}

float GetBoardEval(Board *board) {
    return board->eval;
}

void FenToBoard(const char *fen, Board *board) {
    int row = 7;
    int col = 0;
    int pos;

    int values[12] = {1, -1, 3, -3, 3, -3, 5, -5, 9, -9, 1000, -1000};

    int section = 0;
    char *ptr;
    int enpassant = -1;

    for (int c = 0; c < MAX_FEN_LENGTH; c++) {
        char character = fen[c];
        if (character == ' ') {
            section++;
        }
        if (row >= 0 && section == 0) {
            pos = row * SQUARE_COUNT + col;
            if (strchr(PIECES, character) != NULL) {
                char *e;
                int index;
                e = strchr(PIECES, character);
                index = (int) (e - PIECES);
                board->Board[pos] = (Piece) {index, pos, isupper(character) ? WHITE_PIECE : BLACK_PIECE, (float) values[index]};
                if (character == 'k') {
                    board->kingPos[BLACK_PIECE] = pos;
                } else if (character == 'K') {
                    board->kingPos[WHITE_PIECE] = pos;
                }
                ++col;
            } else if (character != '/') {
                if (!isalpha(character)) {
                    int posChange = character - '0';
                    if (posChange <= 8) {
                        for (int i = 0; i < posChange; i++) {
                            pos = row * SQUARE_COUNT + col;
                            board->Board[pos] = (Piece) {EMPTY, pos, -1, 0.0f};
                            ++col;
                        }
                    } else {
                        exit(-1);
                    }
                }
            } else if (character == '/') {
                --row;
                col = 0;
            }
        } else if (section == 1) {
            if (character == 'w') {
                board->turn = WHITE_PIECE;
            } else {
                board->turn = BLACK_PIECE;
            }
        } else if (section == 2) {
            if (character == 'K') {
                board->castle[0] = 1;
            }
            else if (character == 'Q') {
                board->castle[1] = 1;
            }
            else if (character == 'k') {
                board->castle[2] = 1;
            }
            else if (character == 'q') {
                board->castle[3] = 1;
            }
        } else if (section == 3) {
            char let[8] = "abcdefgh";
            if (character != '-') {
                if (strchr(let, character) != NULL) {
                    for (int i = 0; i < 8; i++) {
                        if (let[i] == character) {
                            enpassant = i * SQUARE_COUNT;
                        }
                    }
                } else {
                    enpassant += (int) strtol(&character, &ptr, 10) - 1;
                }
            }
        } else if (section == 4) {
            board->halfMoveClock = (int) strtol(&character, &ptr, 10);
        } else if (section == 5) {
            board->moveCount = (int) strtol(&character, &ptr, 10);
        }
    }

    board->draw = 0;
}

// Functions for Operating on the Board Struct
Board* CopyBoard(Board *oldBoard) {
    Board *newBoard = CreateBoard();

    // Allocate information
    for (int i = 0; i < SQUARES; i++) {
        newBoard->Board[i] = oldBoard->Board[i];
    }

    // Set other conditions
    newBoard->turn = oldBoard->turn;
    newBoard->movesCount = oldBoard->movesCount;

    for (int i = 0; i < 4; i++) {
        newBoard->castle[i] = oldBoard->castle[i];
    }
    for (int i = 0; i < 2; i++) {
        newBoard->kingPos[i] = oldBoard->kingPos[i];
    }

    newBoard->eval = oldBoard->eval;
    newBoard->enpassant = oldBoard->enpassant;
    newBoard->halfMoveClock = oldBoard->halfMoveClock;
    newBoard->moveCount = oldBoard->movesCount;
    newBoard->checkMate = oldBoard->checkMate;
    newBoard->draw = oldBoard->draw;

    return newBoard;
}

Board* CreateBoard(void) {
    // Creates a board, allocating memory for the pieces
    Board *board = malloc(sizeof (Board));
    if (board == NULL) {
        return NULL;
    }

    board->Board = malloc(sizeof (Piece) * SQUARES);
    if (board->Board == NULL) {
        free(board);
        return NULL;
    }

    // Reasonable values
    board->checkMate = 0; // gets checked on fen
    board->draw = 0;
    board->eval = 0.0f;

    return board;
}

void FreeBoard(Board *board) {
    // Frees the board and the pieces
    free(board->Board);
    board->Board = NULL;
    free(board);
    board = NULL;
}
