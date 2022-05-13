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
    int x = (int) ((mousePosX - boardDimensions->cornerX) / squareWidth);
    int y = SQUARE_COUNT - (int) ((mousePosY - boardDimensions->cornerY) / squareWidth) - 1;

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

void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType) {
    // Check if enpassant
    board->enpassant = -1;
    if (board->Board[pieceSquare].type == 'p' || board->Board[pieceSquare].type == 'P') {
        if (abs(pieceSquare - selected) == SQUARE_COUNT * 2) {
            board->enpassant = (pieceSquare + selected) / 2;
        }
    }
    if (board->Board[pieceSquare].type == 'r' || board->Board[pieceSquare].type == 'R') {
        if (board->Board[pieceSquare].color == 0) {
            if (pieceSquare == SQUARES - 1) {
                board->castle[2] = 0;
            } else if (pieceSquare == SQUARES - SQUARE_COUNT) {
                board->castle[3] = 0;
            }
        } else if (board->Board[pieceSquare].color == 1) {
            if (pieceSquare == SQUARE_COUNT - 1) {
                board->castle[0] = 0;
            } else if (pieceSquare == 0) {
                board->castle[1] = 0;
            }
        }
    }
    if (board->Board[pieceSquare].type == 'k' || board->Board[pieceSquare].type == 'K') {
        board->kingPos[board->Board[pieceSquare].color] = selected;
    }

    Piece *piece = malloc(sizeof (Piece));
    *piece = board->Board[pieceSquare];
    board->Board[pieceSquare] = (Piece) {'e', pieceSquare, -1};
    if (moveType == ENPASSANT) {
        int takenSquare = pieceSquare;
        takenSquare += ((pieceSquare % SQUARE_COUNT - selected % SQUARE_COUNT) == 1 ? -1 : 1);
        board->Board[takenSquare] = (Piece) {'e', takenSquare, -1};
    }
    board->Board[selected] = *piece;
    if (moveType == PROMOTION) {
        if (board->Board[selected].color == 1) {
            board->Board[selected] = (Piece) {'Q', selected, 1};
        } else {
            board->Board[selected] = (Piece) {'q', selected, 0};
        }
    }
    if (moveType == CASTLE) {
        if (board->turn == 1) {
            board->castle[0] = 0;
            board->castle[1] = 0;
        } else {
            board->castle[2] = 0;
            board->castle[3] = 0;
        }
        if (pieceSquare > selected) {
            SwapPieces(board, selected - 2, selected + 1);
        } else {
            SwapPieces(board, selected - 1, selected + 1);
        }
    }
    board->turn = (board->turn + 1) % 2;
    free(piece);
}

void CopyBoard(Board *newBoard, Board *oldBoard) {
    newBoard->Board = malloc(sizeof (Piece) * SQUARES);
    for (int i = 0; i < SQUARES; i++) {
        newBoard->Board[i] = oldBoard->Board[i];
    }
    newBoard->kingPos[0] = oldBoard->kingPos[0];
    newBoard->kingPos[1] = oldBoard->kingPos[1];
    newBoard->turn = oldBoard->turn;
}

void SwapPieces(Board *board, int pos1, int pos2) {
    Piece *tempPiece = malloc(sizeof (Piece));
    *tempPiece = board->Board[pos1];
    board->Board[pos1] = board->Board[pos2];
    board->Board[pos2] = *tempPiece;
    free(tempPiece);
}

void FenToBoard(const char *fen, Board *board) {
    int row = 7;
    int col = 0;
    int pos;

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
                board->Board[pos] = (Piece) {character, pos, isupper(character) ? 1 : 0};
                if (character == 'k') {
                    board->kingPos[0] = pos;
                } else if (character == 'K') {
                    board->kingPos[1] = pos;
                }
                ++col;
            } else if (character != '/') {
                if (!isalpha(character)) {
                    int posChange = character - '0';
                    if (posChange <= 8) {
                        for (int i = 0; i < posChange; i++) {
                            pos = row * SQUARE_COUNT + col;
                            board->Board[pos] = (Piece) {'e', pos, -1};
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
                board->turn = 1;
            } else {
                board->turn = 0;
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
}