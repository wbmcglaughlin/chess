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
        GetRookMoves(board, moves, selected, 1);
    } else if (c == 'b' || c == 'B') {
        GetBishopMoves(board, moves, selected);
    } else if (c == 'q' || c == 'Q') {
        GetRookMoves(board, moves, selected, 0);
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
    if (((int) (selected / SQUARE_COUNT) == 1 && col == 1) || ((int) selected / SQUARE_COUNT == 6 && col == 0)) {
        f2 = selected + dir * 2 * SQUARE_COUNT;
    }

    if (f1 >= 0 && f1 < SQUARES && board->Board[f1].type == 'e') {
        moves[f1] = TO_EMPTY;
        if (f2 >= 0 && f2 < SQUARES && board->Board[f2].type == 'e') {
            moves[f2] = TO_EMPTY;
        }
        if (((int) f1 / SQUARE_COUNT == 0 && col == 0) || ((int) f1 / SQUARE_COUNT == SQUARE_COUNT - 1 && col == 1)) {
            moves[f1] = PROMOTION;
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
        if (((int) fL / SQUARE_COUNT == 0 && col == 0) || ((int) fL / SQUARE_COUNT == SQUARE_COUNT - 1 && col == 1)) {
            moves[fL] = PROMOTION;
        }
    }
    if (PosIsValid(fR) && (int) (selected / SQUARE_COUNT) + dir == (int) fR / SQUARE_COUNT) {
        if (board->Board[fR].color == (col + 1) % 2) {
            moves[fR] = CAPTURE;
        } else if (fR == board->enpassant) {
            moves[fR] = ENPASSANT;
        }
        if (((int) fR / SQUARE_COUNT == 0 && col == 0) || ((int) fR / SQUARE_COUNT == SQUARE_COUNT - 1 && col == 1)) {
            moves[fR] = PROMOTION;
        }
    }

    UpdateMovesLegality(board, moves, selected);
}

void GetRookMoves(Board *board, int *moves, int selected, int findLegal) {
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
        while (selected / SQUARE_COUNT == pos / SQUARE_COUNT && !blocked && PosIsValid(pos)) {
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

    if (findLegal) {
        UpdateMovesLegality(board, moves, selected);
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

    UpdateMovesLegality(board, moves, selected);
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

    int isempty = 1;
    if (col == 1) {
        // King Side Castle White
        if (board->castle[0] == 1) {
            for (int i = board->kingPos[1] + 1; i < SQUARE_COUNT - 1; i++) {
                if (board->Board[i].type != 'e') {
                    isempty = 0;
                }
            }
            if (isempty == 1) {
                moves[SQUARE_COUNT - 2] = CASTLE;
            }
        }
        // Queen Side Castle White
        isempty = 1;
        if (board->castle[1] == 1) {
            for (int i = board->kingPos[1] - 1; i > 1; i--) {
                if (board->Board[i].type != 'e') {
                    isempty = 0;
                }
            }
            if (isempty == 1) {
                moves[2] = CASTLE;
            }
        }
    } else if (col == 0) {
        // King Side Castle Black
        isempty = 1;
        if (board->castle[2] == 1) {
            for (int i = board->kingPos[0] + 1; i < SQUARES - 1; i++) {
                if (board->Board[i].type != 'e') {
                    isempty = 0;
                }
            }
            if (isempty == 1) {
                moves[SQUARES - 2] = CASTLE;
            }
        }
        // Queen Side Castle Black
        isempty = 1;
        if (board->castle[3] == 1) {
            for (int i = board->kingPos[0] - 1; i > SQUARES - SQUARE_COUNT + 1; i--) {
                if (board->Board[i].type != 'e') {
                    isempty = 0;
                }
            }
            if (isempty == 1) {
                moves[SQUARES - SQUARE_COUNT + 2] = CASTLE;
            }
        }
    }

    UpdateMovesLegality(board, moves, selected);
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

    for (int i = 0; i < SQUARES; i++) {
        if (moves[i] > 0) {
            if (!IsMoveLegal(board, selected, i, moves[i])) {
                moves[i] = 0;
            }
        }
    }

    UpdateMovesLegality(board, moves, selected);
}

void UpdateMovesLegality(Board *board, int *moves, int selected) {
    for (int i = 0; i < SQUARES; i++) {
        if (moves[i] > 0) {
            if (!IsMoveLegal(board, selected, i, moves[i])) {
                moves[i] = 0;
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

int IsMoveLegal(Board *board, int selected, int move, int moveType) {
    int col = board->turn;
    Board *boardUpdated = malloc(sizeof (*boardUpdated));;
    CopyBoard(boardUpdated, board);
    UpdateBoard(boardUpdated, selected, move, moveType);
    if (!KingInCheck(boardUpdated, col)) {
        return 1;
    }
    free(boardUpdated);
    return 0;
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

int KingInCheck(Board *board, int col) {
    int kingPos = board->kingPos[col];

    // Check For Knights
    char type = 'n';
    if (col == 0) {
        type = (char) toupper(type);
    }
    int dir[4] = {-SQUARE_COUNT - 2, -SQUARE_COUNT + 2, -2*SQUARE_COUNT - 1, -2*SQUARE_COUNT + 1};
    int rowDif[4] = {-2, 2, -1, 1};
    int pos;
    for (int i = 0; i < 4; i++) {
        pos = kingPos + dir[i];
        if (PosIsValid(pos) && kingPos % SQUARE_COUNT - pos % SQUARE_COUNT == -rowDif[i]) {
            if (board->Board[pos].type == type) {
                return 1;
            }
        }

        pos = kingPos - dir[i];
        if (PosIsValid(pos) && kingPos % SQUARE_COUNT - pos % SQUARE_COUNT == rowDif[i]) {
            if (board->Board[pos].type == type) {
                return 1;
            }
        }
    }

    // Check For Rooks or Queens
    type = 'r';
    char type2 = 'q';
    if (col == 0) {
        type = (char) toupper(type);
        type2 = (char) toupper(type2);
    }
    int dirUpDown[2] = {SQUARE_COUNT, -SQUARE_COUNT};
    int dirLeftRight[2] = {1, -1};

    for (int i = 0; i < 2; i++) {
        pos = kingPos + dirUpDown[i];
        while (PosIsValid(pos)) {
            if (board->Board[pos].type == 'e') {
                pos += dirUpDown[i];
                continue;
            } else if (board->Board[pos].type == type || board->Board[pos].type == type2) {
                return 1;
            } else {
                break;
            }
        }

        pos = kingPos + dirLeftRight[i];
        while (kingPos / SQUARE_COUNT == pos / SQUARE_COUNT && PosIsValid(pos)) {
            if (board->Board[pos].type == 'e') {
                pos += dirLeftRight[i];
                continue;
            } else if (board->Board[pos].type == type || board->Board[pos].type == type2) {
                return 1;
            } else {
                break;
            }
        }
    }

    // Check for Bishop
    type = 'b';
    if (col == 0) {
        type = (char) toupper(type);
    }
    int dirBishop[4] = {SQUARE_COUNT + 1, SQUARE_COUNT - 1,- SQUARE_COUNT + 1, - SQUARE_COUNT - 1};
    int correctDiagDir[4] = {1, -1, 1, -1};

    for (int i = 0; i < 4; i++) {
        int pos_old = kingPos;
        pos = kingPos + dirBishop[i];
        int blocked = 0;
        int diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        while (PosIsValid(pos) && !blocked && diagDir == correctDiagDir[i]) {
            if (board->Board[pos].type == 'e') {
                pos_old = pos;
                pos += dirBishop[i];
                diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
                continue;
            } else if (board->Board[pos].type == type) {
                return 1;
            } else {
                break;
            }
        }
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
