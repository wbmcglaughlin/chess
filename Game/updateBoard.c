//
// Created by Will McGlaughlin on 27/8/2022.
//

#include "updateBoard.h"


void UpdateBoard(Board *board, int pieceSquare, int selected, int moveType) {
    int values[12] = {1, -1, 3, -3, 3, -3, 5, -5, 9, -9, 1000, -1000};

    if (board->Board[pieceSquare].type == BLACK_PAWN || board->Board[pieceSquare].type == WHITE_PAWN  || moveType == CAPTURE) {
        board->halfMoveClock = 0;
    } else {
        board->halfMoveClock += 1;
    }

    // Check if enpassant
    board->enpassant = -1;
    if (board->Board[pieceSquare].type == BLACK_PAWN || board->Board[pieceSquare].type == WHITE_PAWN) {
        if (abs(pieceSquare - selected) == SQUARE_COUNT * 2) {
            board->enpassant = (pieceSquare + selected) / 2;
        }
    }
    if (board->Board[pieceSquare].type == BLACK_ROOK || board->Board[pieceSquare].type == WHITE_ROOK) {
        if (board->Board[pieceSquare].color == BLACK_PIECE) {
            if (pieceSquare == SQUARES - 1) {
                board->castle[2] = 0;
            } else if (pieceSquare == SQUARES - SQUARE_COUNT) {
                board->castle[3] = 0;
            }
        } else if (board->Board[pieceSquare].color == WHITE_PIECE) {
            if (pieceSquare == SQUARE_COUNT - 1) {
                board->castle[0] = 0;
            } else if (pieceSquare == 0) {
                board->castle[1] = 0;
            }
        }
    }
    if (board->Board[pieceSquare].type == BLACK_KING || board->Board[pieceSquare].type == WHITE_KING) {
        board->kingPos[board->Board[pieceSquare].color] = selected;
        if (board->Board[pieceSquare].type == BLACK_KING) {
            board->castle[2] = 0;
            board->castle[3] = 0;
        } else {
            board->castle[0] = 0;
            board->castle[1] = 0;
        }
    }

    Piece *piece = malloc(sizeof (Piece));
    *piece = board->Board[pieceSquare];
    board->Board[pieceSquare] = (Piece) {EMPTY, pieceSquare, -1};
    if (moveType == ENPASSANT) {
        int takenSquare = pieceSquare;
        takenSquare += ((pieceSquare % SQUARE_COUNT - selected % SQUARE_COUNT) == 1 ? -1 : 1);
        board->Board[takenSquare] = (Piece) {EMPTY, takenSquare, -1};
    }
    board->Board[selected] = *piece;
    if (moveType == PROMOTION) {
        if (board->Board[selected].color == WHITE_PIECE) {
            board->Board[selected] = (Piece) {WHITE_QUEEN, selected, WHITE_PIECE, (float) values[WHITE_QUEEN]};
        } else {
            board->Board[selected] = (Piece) {BLACK_QUEEN, selected, BLACK_PIECE, (float) values[BLACK_QUEEN]};
        }
    }
    if (moveType == CASTLE) {
        if (board->turn == WHITE_PIECE) {
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

    board->moveCount++;
    board->turn = (board->turn + 1) % 2;

    // Cleaning Up
    free(piece);
}

void CheckStatus(Board *board) {
    int newMoves = GetAllLegalMovesCount(board);

    if (newMoves == 0) {
        if (!IsKingInCheck(board, board->turn)) {
            board->draw = 1;
        } else {
            board->checkMate = 1;
        }
    }

    if (board->halfMoveClock >= 50) {
        board->draw = 1;
    }
}