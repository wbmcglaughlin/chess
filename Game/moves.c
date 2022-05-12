//
// Created by Will McGlaughlin on 13/5/2022.
//

#include "moves.h"

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

int IsMoveLegal(Board *board, int selected, int move, int moveType) {
    int col = board->turn;
    Board *boardUpdated = malloc(sizeof (*boardUpdated));;
    CopyBoard(boardUpdated, board);
    UpdateBoard(boardUpdated, selected, move, moveType);
    if (!IsKingInCheck(boardUpdated, col)) {
        return 1;
    }
    free(boardUpdated);
    return 0;
}

int IsKingInCheck(Board *board, int col) {
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


