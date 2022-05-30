//
// Created by Will McGlaughlin on 30/5/2022.
//

#include "update.h"

void BoardUpdateLoop(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld) {
    if (!board->checkMate) {
        if (board->turn == PLAYER) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (*selected != -1) {
                    *pieceHeld = 1;
                } else {
                    GetSelected(selected,
                                GetMousePosition().x,
                                GetMousePosition().y,
                                boardDimensions);
                    if (board->Board[*selected].color == board->turn) {
                        ClearMoves(moveSquares);
                        GetMoves(board, moveSquares, *selected);
                    }
                    *getMoves = (*getMoves + 1) % 2;
                    if (board->Board[*selected].color != board->turn) {
                        *selected = -1;
                    }
                }
            } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) && pieceHeld) {
                int pieceSquare = *selected;
                GetSelected(selected,
                            GetMousePosition().x,
                            GetMousePosition().y,
                            boardDimensions);

                // If valid square and not the same square
                if (*selected != -1 && *selected != pieceSquare && moveSquares[*selected] > 0) {
                    UpdateBoard(board, pieceSquare, *selected, moveSquares[*selected]);
                }
                pieceHeld = 0;
                *getMoves = (*getMoves + 1) % 2;
                ClearMoves(moveSquares);
                *selected = -1;
            }
        } else {
            Move botMove = CaptureFirstBot(board);
            UpdateBoard(board, botMove.pos, botMove.target, botMove.moveType);
            board->turn = PLAYER;
        }
        GetAllLegalMoves(board, moves, movesCount);

        if (movesCount == 0) {
            board->checkMate = 1;
        }
    }
}

