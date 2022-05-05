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

void GetSelected(int *selected, float mousePosX, float mousePosY, int cornerX, int cornerY, int sideSize) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;

    // mousePosX = cornerX + x * squareWidth
    // mousePosY = cornerY + (SQUARE_COUNT - y - 1) * squareWidth;
    int x = (int) ((mousePosX - cornerX) / squareWidth);
    int y = SQUARE_COUNT - (int) ((mousePosY - cornerY) / squareWidth) - 1;

    if (x >= 0 && x < SQUARE_COUNT && y >= 0 && y < SQUARE_COUNT) {
        *selected = x + SQUARE_COUNT * y;
    }
}

void GetMoves(Board *board, int *moves, int selected) {
    char c = board->Board[selected].type;
    if (c == 'p' || c == 'P') {
        GetPawnMoves(board, moves, selected);
    } else if (c == 'r' || c == 'R') {
        GetRookMoves(board, moves, selected);
    } else if (c == 'b' || c == 'B') {
        GetBishopMoves(board, moves, selected);
    } else if (c == 'q' || c == 'Q') {
        GetRookMoves(board, moves, selected);
        GetBishopMoves(board, moves, selected);
    } else if (c == 'k' || c == 'K') {
        GetKingMoves(board, moves, selected);
    } else if (c == 'n' || c == 'N') {
        GetKnightMoves(board, moves, selected);
    }
}

void GetPawnMoves(Board *board, int *moves, int selected) {
    int dir = 1;
    int col = board->Board[selected].color;
    if (col == 0) {
        dir = -1;
    }

    int f1 = selected + dir * SQUARE_COUNT;
    int f2 = -1;
    if (((int) selected / SQUARE_COUNT == 1 && col == 1) || ((int) selected / SQUARE_COUNT == 6 && col == 0)) {
        f2 = selected + dir * 2 * SQUARE_COUNT;
    }

    if (f1 >= 0 && f1 < SQUARES && board->Board[f1].type == 'e') {
        moves[f1] = TO_EMPTY;
        if (f2 >= 0 && f2 < SQUARES && board->Board[f2].type == 'e') {
            moves[f2] = TO_EMPTY;
        }
    }

    int fL = selected + dir * SQUARE_COUNT - dir;
    int fR = selected + dir * SQUARE_COUNT + dir;
    if (PosIsValid(fL) && (int) (selected / SQUARE_COUNT) + dir == (int) fL / SQUARE_COUNT) {
        if (board->Board[fL].color == (col + 1) % 2) {
            moves[fL] = CAPTURE;
        } else if (fL == board->enpassant) {
            moves[fL] = ENPASSANT;
        }
    }
    if (PosIsValid(fR) && (int) (selected / SQUARE_COUNT) + dir == (int) fR / SQUARE_COUNT) {
        if (board->Board[fR].color == (col + 1) % 2) {
            moves[fR] = CAPTURE;
        } else if (fR == board->enpassant) {
            moves[fR] = ENPASSANT;
        }
    }
}

void GetRookMoves(Board *board, int *moves, int selected) {
    int dirUpDown[2] = {SQUARE_COUNT, -SQUARE_COUNT};
    int dirLeftRight[2] = {1, -1};
    int col = board->Board[selected].color;

    for (int i = 0; i < 2; i++) {
        int pos = selected + dirUpDown[i];
        int blocked = 0;
        while (PosIsValid(pos) && !blocked) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
                blocked = 1;
            } else {
                blocked = 1;
            }
            pos += dirUpDown[i];
        }

        pos = selected + dirLeftRight[i];
        blocked = 0;
        while (selected / SQUARE_COUNT == pos / SQUARE_COUNT && !blocked) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
                blocked = 1;
            } else {
                blocked = 1;
            }
            pos += dirLeftRight[i];
        }
    }
}

void GetBishopMoves(Board *board, int *moves, int selected) {
    int dir[4] = {SQUARE_COUNT + 1, SQUARE_COUNT - 1,- SQUARE_COUNT + 1, - SQUARE_COUNT - 1};
    int correctDiagDir[4] = {1, -1, 1, -1};
    int col = board->Board[selected].color;

    for (int i = 0; i < 4; i++) {
        int pos_old = selected;
        int pos = selected + dir[i];
        int blocked = 0;
        int diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        while (PosIsValid(pos) && !blocked && diagDir == correctDiagDir[i]) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
                blocked = 1;
            } else {
                blocked = 1;
            }
            pos_old = pos;
            pos += dir[i];
            diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        }
    }
}

void GetKingMoves(Board *board, int *moves, int selected) {
    int dirDiag[4] = {SQUARE_COUNT + 1, SQUARE_COUNT - 1,- SQUARE_COUNT + 1, - SQUARE_COUNT - 1};
    int correctDiagDir[4] = {1, -1, 1, -1};
    int col = board->Board[selected].color;

    for (int i = 0; i < 4; i++) {
        int pos_old = selected;
        int pos = selected + dirDiag[i];

        int diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        if (PosIsValid(pos) && diagDir == correctDiagDir[i]) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }
    }

    int dirUpDown[2] = {SQUARE_COUNT, -SQUARE_COUNT};
    int dirLeftRight[2] = {1, -1};

    for (int i = 0; i < 2; i++) {
        int pos = selected + dirUpDown[i];
        if (PosIsValid(pos)) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }

        pos = selected + dirLeftRight[i];
        if (selected / SQUARE_COUNT == pos / SQUARE_COUNT) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }
    }
}

void GetKnightMoves(Board *board, int *moves, int selected) {
    int col = board->Board[selected].color;
    int dir[4] = {-SQUARE_COUNT - 2, -SQUARE_COUNT + 2, -2*SQUARE_COUNT - 1, -2*SQUARE_COUNT + 1};
    int rowDif[4] = {-2, 2, -1, 1};
    int pos;
    for (int i = 0; i < 4; i++) {
        pos = selected + dir[i];
        if (PosIsValid(pos) && selected % SQUARE_COUNT - pos % SQUARE_COUNT == -rowDif[i]) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }

        pos = selected - dir[i];
        if (PosIsValid(pos) && selected % SQUARE_COUNT - pos % SQUARE_COUNT == rowDif[i]) {
            if (board->Board[pos].type == 'e') {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }
    }
}

int PosIsValid(int pos) {
    if (pos < SQUARES && pos >= 0) {
        return 1;
    }
    return 0;
}

void ClearMoves(int *moves) {
    for (int i = 0; i < SQUARES; i++) {
        moves[i] = 0;
    }
}

void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType) {
    // Check if enpassant
    board->enpassant = -1 ;
    if (board->Board[pieceSquare].type == 'p' || board->Board[pieceSquare].type == 'P') {
        if (abs(pieceSquare - selected) == SQUARE_COUNT * 2) {
            board->enpassant = (pieceSquare + selected) / 2;
        }
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
    board->turn = (board->turn + 1) % 2;

    free(piece);
}


void DrawBoard(int cornerX, int cornerY, int sideSize, int *moves, int selected) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, cornerX, cornerY);
        DrawRectangle(boardSquare->cornerX,
                      boardSquare->cornerY,
                      squareWidth,
                      squareWidth,
                      ((boardSquare->x + boardSquare->y) % 2 == 0) ? DARKGRAY : WHITE
                      );

        if (i == selected) {
            DrawText(TextFormat("Selected: %i", i), 400, 5, 20, BLACK);
            DrawRectangle(boardSquare->cornerX,
                          boardSquare->cornerY,
                          squareWidth,
                          squareWidth,
                          GREEN
                          );
        }
        if (moves[i] > 0) {
            DrawRectangle(boardSquare->cornerX,
                          boardSquare->cornerY,
                          squareWidth,
                          squareWidth,
                          LIGHTGRAY
            );
        }
        free(boardSquare);
    }
}

void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, cornerX, cornerY);
        char type = board->Board[i].type;
        if (type != 'e') {
            for (int j = 0; j < UNIQUE_PIECE_TEXTURES; j++) {
                if (type == PIECES[j]) {
                    if (pieceHeld && i == selected) {
                        DrawTextureEx(*pieceTextures[j],
                                   (Vector2) {mousePosition.x - squareWidth / 2, mousePosition.y - squareWidth / 2},
                                   0,
                                   squareWidth / (float) (*pieceTextures[0]).height,
                                   WHITE);
                    } else {
                        DrawTextureEx(*pieceTextures[j],
                                      (Vector2) {boardSquare->cornerX, boardSquare->cornerY},
                                      0,
                                      squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    }
                }
            }
            free(boardSquare);
        }
    }
}

void FenToBoard(const char *fen, Board *board) {
    int row = 7;
    int col = 0;
    int pos;

    for (int c = 0; c < MAX_FEN_LENGTH; c++) {
        char ch = fen[c];
        if (row >= 0) {
            if (ch == ' ') {
                --row;
                break;
            }
            pos = row * SQUARE_COUNT + col;
            if (strchr(PIECES, ch) != NULL) {
                board->Board[pos] = (Piece) {ch, pos, isupper(ch) ? 1 : 0};
                ++col;
            } else if (ch != '/') {
                int pos_change = atoi(&ch);
                for (int i = 0; i < pos_change; i++) {
                    pos = row * SQUARE_COUNT + col;
                    board->Board[pos] = (Piece) {'e', pos, -1};
                    ++col;
                }
            } else if (ch == '/') {
                --row;
                col = 0;
            }
        }
    }
}
