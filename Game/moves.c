//
// Created by Will McGlaughlin on 13/5/2022.
//

#include "moves.h"

void GetMoves(Board *board, int *moves, int selected) {
    int c = board->Board[selected].type;
    if (c == BLACK_PAWN || c == WHITE_PAWN) {
        GetPawnMoves(board, moves, selected);
    } else if (c == BLACK_ROOK || c == WHITE_ROOK) {
        GetRookMoves(board, moves, selected, 1);
    } else if (c == BLACK_BISHOP || c == WHITE_BISHOP) {
        GetBishopMoves(board, moves, selected);
    } else if (c == BLACK_QUEEN || c == WHITE_QUEEN) {
        GetRookMoves(board, moves, selected, 0);
        GetBishopMoves(board, moves, selected);
    } else if (c == BLACK_KING || c == WHITE_KING) {
        GetKingMoves(board, moves, selected);
    } else if (c == BLACK_KNIGHT || c == WHITE_KNIGHT) {
        GetKnightMoves(board, moves, selected);
    }
}

void GetPawnMoves(Board *board, int *moves, int selected) {
    int dir = 1;
    int col = board->Board[selected].color;
    if (col == BLACK_PIECE) {
        dir = -1;
    }

    int f1 = selected + dir * SQUARE_COUNT;
    int f2 = -1;
    if (((int) (selected / SQUARE_COUNT) == 1 && col == WHITE_PIECE) || ((int) selected / SQUARE_COUNT == 6 && col == BLACK_PIECE)) {
        f2 = selected + dir * 2 * SQUARE_COUNT;
    }

    if (f1 >= 0 && f1 < SQUARES && board->Board[f1].type == EMPTY) {
        moves[f1] = TO_EMPTY;
        if (f2 >= 0 && f2 < SQUARES && board->Board[f2].type == EMPTY) {
            moves[f2] = TO_EMPTY;
        }
        if (((int) f1 / SQUARE_COUNT == 0 && col == BLACK_PIECE) || ((int) f1 / SQUARE_COUNT == SQUARE_COUNT - 1 && col == WHITE_PIECE)) {
            moves[f1] = PROMOTION;
        }
    }

    int fL = selected + dir * SQUARE_COUNT - dir;
    int fR = selected + dir * SQUARE_COUNT + dir;
    if (PosIsValid(fL) && (int) (selected / SQUARE_COUNT) + dir == (int) fL / SQUARE_COUNT) {
        if (board->Board[fL].color == (col + 1) % 2) {
            moves[fL] = CAPTURE;
            if (((int) fL / SQUARE_COUNT == 0 && col == BLACK_PIECE) || ((int) fL / SQUARE_COUNT == SQUARE_COUNT - 1 && col == WHITE_PIECE)) {
                moves[fL] = PROMOTION;
            }
        } else if (fL == board->enpassant) {
            moves[fL] = ENPASSANT;
        }
    }
    if (PosIsValid(fR) && (int) (selected / SQUARE_COUNT) + dir == (int) fR / SQUARE_COUNT) {
        if (board->Board[fR].color == (col + 1) % 2) {
            moves[fR] = CAPTURE;
            if (((int) fR / SQUARE_COUNT == 0 && col == BLACK_PIECE) || ((int) fR / SQUARE_COUNT == SQUARE_COUNT - 1 && col == WHITE_PIECE)) {
                moves[fR] = PROMOTION;
            }
        } else if (fR == board->enpassant) {
            moves[fR] = ENPASSANT;
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
            if (board->Board[pos].type == EMPTY) {
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
            if (board->Board[pos].type == EMPTY) {
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
            if (board->Board[pos].type == EMPTY) {
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
            if (board->Board[pos].type == EMPTY) {
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
            if (board->Board[pos].type == EMPTY) {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }

        pos = selected + dirLeftRight[i];
        if (selected / SQUARE_COUNT == pos / SQUARE_COUNT) {
            if (board->Board[pos].type == EMPTY) {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }
    }

    int isempty = 1;
    if (col == WHITE_PIECE && !IsKingInCheck(board, WHITE_PIECE)) {
        // King Side Castle White
        if (board->castle[0] == 1) {
            for (int i = board->kingPos[WHITE_PIECE] + 1; i < SQUARE_COUNT - 1; i++) {
                if (board->Board[i].type != EMPTY || !IsMoveLegal(board, board->kingPos[WHITE_PIECE], i, TO_EMPTY)) {
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
            for (int i = board->kingPos[WHITE_PIECE] - 1; i > 1; i--) {
                if (board->Board[i].type != EMPTY || !IsMoveLegal(board, board->kingPos[WHITE_PIECE], i, TO_EMPTY)) {
                    isempty = 0;
                }
            }
            if (isempty == 1) {
                moves[2] = CASTLE;
            }
        }
    } else if (col == BLACK_PIECE && !IsKingInCheck(board, BLACK_PIECE)) {
        // King Side Castle Black
        isempty = 1;
        if (board->castle[2] == 1) {
            for (int i = board->kingPos[BLACK_PIECE] + 1; i < SQUARES - 1; i++) {
                if (board->Board[i].type != EMPTY || !IsMoveLegal(board, board->kingPos[BLACK_PIECE], i, TO_EMPTY)) {
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
            for (int i = board->kingPos[BLACK_PIECE] - 1; i > SQUARES - SQUARE_COUNT + 1; i--) {
                if (board->Board[i].type != EMPTY || !IsMoveLegal(board, board->kingPos[BLACK_PIECE], i, TO_EMPTY)) {
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
            if (board->Board[pos].type == EMPTY) {
                moves[pos] = TO_EMPTY;
            } else if (board->Board[pos].color != col) {
                moves[pos] = CAPTURE;
            }
        }

        pos = selected - dir[i];
        if (PosIsValid(pos) && selected % SQUARE_COUNT - pos % SQUARE_COUNT == rowDif[i]) {
            if (board->Board[pos].type == EMPTY) {
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

void ClearMoves(int *moves) {
    for (int i = 0; i < SQUARES; i++) {
        moves[i] = 0;
    }
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

int IsKingInCheck(Board *board, int color) {
    ///
    /// board: current board
    ///
    int col = color;
    int kingPos = board->kingPos[col];

    // Check for Pawns
    char type = BLACK_PAWN;
    if (col == BLACK_PIECE) {
        type = WHITE_PAWN;
    }
    int dirPawn = 1;
    if (col == BLACK_PIECE) {
        dirPawn = -1;
    }
    int fL = kingPos + dirPawn * SQUARE_COUNT - dirPawn;
    int fR = kingPos + dirPawn * SQUARE_COUNT + dirPawn;
    if (PosIsValid(fL) && (int) (kingPos / SQUARE_COUNT) + dirPawn == (int) fL / SQUARE_COUNT) {
        if (board->Board[fL].type == type) {
            return 1;
        }
    }
    if (PosIsValid(fR) && (int) (kingPos / SQUARE_COUNT) + dirPawn == (int) fR / SQUARE_COUNT) {
        if (board->Board[fR].type == type) {
            return 1;
        }
    }

    // Check For Knights
    type = BLACK_KNIGHT;
    if (col == BLACK_PIECE) {
        type = WHITE_KNIGHT;
    }
    int dir[4] = {-SQUARE_COUNT - 2, -SQUARE_COUNT + 2, -2 * SQUARE_COUNT - 1, -2 * SQUARE_COUNT + 1};
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
    type = BLACK_ROOK;
    char type2 = BLACK_QUEEN;
    if (col == BLACK_PIECE) {
        type = WHITE_ROOK;
        type2 = WHITE_QUEEN;
    }
    int dirUpDown[2] = {SQUARE_COUNT, -SQUARE_COUNT};
    int dirLeftRight[2] = {1, -1};

    for (int i = 0; i < 2; i++) {
        pos = kingPos + dirUpDown[i];
        while (PosIsValid(pos)) {
            if (board->Board[pos].type == EMPTY) {
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
            if (board->Board[pos].type == EMPTY) {
                pos += dirLeftRight[i];
                continue;
            } else if (board->Board[pos].type == type || board->Board[pos].type == type2) {
                return 1;
            } else {
                break;
            }
        }
    }

    // Check for Bishop or Queens
    type = BLACK_BISHOP;
    type2 = BLACK_QUEEN;
    if (col == BLACK_PIECE) {
        type = WHITE_BISHOP;
        type2 = WHITE_QUEEN;
    }
    int dirBishop[4] = {SQUARE_COUNT + 1, SQUARE_COUNT - 1,- SQUARE_COUNT + 1, - SQUARE_COUNT - 1};
    int correctDiagDir[4] = {1, -1, 1, -1};

    for (int i = 0; i < 4; i++) {
        int pos_old = kingPos;
        pos = kingPos + dirBishop[i];

        int diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        while (PosIsValid(pos) && diagDir == correctDiagDir[i]) {
            if (board->Board[pos].type == EMPTY) {
                pos_old = pos;
                pos += dirBishop[i];
                diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
                continue;
            } else if (board->Board[pos].type == type || board->Board[pos].type == type2) {
                return 1;
            } else {
                break;
            }
        }
    }

    type = BLACK_KING;
    if (col == BLACK_PIECE) {
        type = WHITE_KING;
    }
    int kingDirDiag[4] = {SQUARE_COUNT + 1, SQUARE_COUNT - 1,- SQUARE_COUNT + 1, - SQUARE_COUNT - 1};

    for (int i = 0; i < 4; i++) {
        int pos_old = kingPos;
        pos = kingPos + kingDirDiag[i];

        int diagDir = (pos % SQUARE_COUNT - pos_old % SQUARE_COUNT);
        if (PosIsValid(pos) && diagDir == correctDiagDir[i]) {
            if (board->Board[pos].type == type) {
                return 1;
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        pos = kingPos + dirUpDown[i];
        if (PosIsValid(pos)) {
            if (board->Board[pos].type == type) {
                return 1;
            }
        }

        pos = kingPos + dirLeftRight[i];
        if (kingPos / SQUARE_COUNT == pos / SQUARE_COUNT) {
            if (board->Board[pos].type == type) {
                return 1;
            }
        }
    }

    return 0;
}

void GetAllLegalMoves(Board *board, Move *moves, int *movesCount) {
    *movesCount = 0;
    int *movesArr = malloc(SQUARES * sizeof (int));
    int movesCountInner = 0;

    ClearMoves(movesArr);

    for (int i = 0; i < SQUARES; i++) {
        if (board->Board[i].type != EMPTY && board->Board[i].color == board->turn) {
            GetMoves(board, movesArr, i);
            for (int j = 0; j < SQUARES; j++) {
                if (movesArr[j] != 0) {
                    moves[movesCountInner] = (Move) {i, j, movesArr[j]};
                    movesCountInner++;
                }
            }
            ClearMoves(movesArr);
        }
    }

    *movesCount = movesCountInner;
    free(movesArr);
}

int MoveSortFunc(const void *f, const void *s) {
    Move *move1 = (Move*)f;
    Move *move2 = (Move*)s;
    return (int) -(move1->moveType - move2->moveType);
}

void GetAllLegalMovesSorted(Board *board, Move *moves, int *movesCount) {
    GetAllLegalMoves(board, moves, movesCount);

    if (*movesCount > 1) {
        qsort(moves, *movesCount, sizeof (Move), &MoveSortFunc);
    }
}

int IsMoveLegal(Board *board, int selected, int move, int moveType) {
    int col = board->turn;
    Board *boardUpdated = CopyBoard(board);
    UpdateBoard(boardUpdated, selected, move, moveType);

    if (!IsKingInCheck(boardUpdated, col)) {
        FreeBoard(boardUpdated);
        return 1;
    }
    FreeBoard(boardUpdated);
    return 0;
}

int GetAllLegalMovesCount(Board *board) {
    // Function to get all moves in a position
    Move *moves = malloc(MAX_MOVES * sizeof (Move));
    int movesCount = 0;

    GetAllLegalMoves(board, moves, &movesCount);
    free(moves);

    return movesCount;
}
