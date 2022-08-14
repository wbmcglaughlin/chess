//
// Created by Will McGlaughlin on 30/5/2022.
//

#include "update.h"

void BoardUpdateLoop(Board *board, BoardDimensions *boardDimensions, BotInput *botInput, int *threadStarted, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld) {
    // Update Board Check
    if (board->checkMate) {
        return;
    }

    if (board->turn == PLAYER) {
        PlayerTurnCheck(board, boardDimensions, moveSquares, movesCount, moves, getMoves, selected, pieceHeld);
    } else {
        BotTurnCheck(botInput, threadStarted, movesCount);
    }

    if (GetAllMovesCount(board) == 0) {
        board->checkMate = 1;
    }
}

void BotTurnCheck(BotInput *botInput, int *threadStarted, int *movesCount) {
    if (!*threadStarted) {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, MiniMaxBot, (void *)botInput);

        *threadStarted = 1;
        return;
    }

    if (!*botInput->hasMove) {
        return;
    }

    if (*botInput->hasMove) {
        Move botMove = *botInput->move;
        UpdateBoard(botInput->board, botMove.pos, botMove.target, botMove.moveType);
        *movesCount = 0;
        Move *movesArr = malloc(SQUARES * sizeof(Move));
        *movesCount = GetAllLegalMovesToDepthCount(botInput->board, DEPTH_SEARCH);
        free(movesArr);
        botInput->moveEval->eval = botInput->moveEval->eval;
        botInput->board->turn = PLAYER;

        *botInput->hasMove = 0;
        *threadStarted = 0;
    }
}

void PlayerTurnCheck(Board *board, BoardDimensions *boardDimensions, int *moveSquares, int *movesCount, Move *moves,
                     int *getMoves, int *selected, int *pieceHeld) {
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

        *getMoves = (*getMoves + 1) % 2;
        ClearMoves(moveSquares);
        *selected = -1;
    }
}

