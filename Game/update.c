//
// Created by Will McGlaughlin on 30/5/2022.
//

#include "update.h"

void BoardUpdateLoop(Game *gameInstance, BoardDimensions *boardDimensions,
                     int *getMoves, int *selected, int *pieceHeld) {
    // Update Board Check
    if (gameInstance->board->checkMate || gameInstance->board->draw) {
        return;
    }

    if (gameInstance->players[gameInstance->board->turn] == PLAYER) {
        PlayerTurnCheck(gameInstance->board, boardDimensions, gameInstance->moveSquares, &gameInstance->movesCount, gameInstance->moves, getMoves, selected, pieceHeld);
    } else {
        BotTurnCheck(gameInstance, boardDimensions, &gameInstance->movesCount);
    }

    if (GetAllMovesCount(gameInstance->board) == 0) {
        gameInstance->board->checkMate = 1;
    }
}

void BotTurnCheck(Game *gameInstance, BoardDimensions *boardDimensions, int *movesCount) {
    // Bot runs on a different thread
    BotInput *botInput = &gameInstance->botInput;

    // Check if the thread has started, if not, start the thread.
    if (!*botInput->threadStarted) {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, MiniMaxBot, (void *)botInput);

        *botInput->threadStarted = 1;
        *botInput->threadRunning = 1;
        return;
    }

    // Checks if the bot has found the move it wants to play, if not, return.
    if (!*botInput->hasMove) {
        // Current best move is displayed to screen.
        DrawArrow(botInput->move->pos, botInput->move->target, ORANGE, boardDimensions);
        return;
    }

    // If the bot has a move, update the board.
    if (*botInput->hasMove) {
        Move botMove = *botInput->move;
        UpdateBoard(botInput->board, botMove.pos, botMove.target, botMove.moveType);
        gameInstance->eval = *botInput->eval;

//        *movesCount = 0;
//        Move *movesArr = malloc(SQUARES * sizeof(Move));
//        *movesCount = GetAllLegalMovesToDepthCount(botInput->board, DEPTH_SEARCH);
//        free(movesArr);

        *botInput->hasMove = 0;
        *botInput->threadStarted = 0;
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
            board->moveCount++;
        }

        *getMoves = (*getMoves + 1) % 2;
        ClearMoves(moveSquares);
        *selected = -1;
    }
}

