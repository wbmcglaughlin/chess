//
// Created by wbmcg on 15/05/2022.
//

#include "bot.h"

BotInput *CreateBotInput(Board *board) {
    BotInput *botInput = malloc(sizeof (BotInput));
    botInput->board = board;

    Move *move = malloc(sizeof (Move));
    *move = (Move) {-1, -1, -1};
    botInput->move = move;

    botInput->eval = malloc(sizeof (float ));
    *botInput->eval = 0.0f;
    botInput->hasMove = malloc(sizeof (int ));
    *botInput->hasMove = 0;
    botInput->threadStarted = malloc(sizeof (int ));
    *botInput->threadStarted = 0;
    botInput->threadRunning = malloc(sizeof (int ));
    *botInput->threadRunning = 0;

    return botInput;
}

void FreeBotInput(BotInput *botInput) {
    free(botInput->threadStarted);
    free(botInput->hasMove);
    free(botInput->move);
}

void* MiniMaxBot(void *botInput) {
    BotInput *botInputStruct = (BotInput*) botInput;
    Board *board = botInputStruct->board;

    MoveEval *moveEval = GetEmptyMoveEval();

    int gamePhase = 0;
    for (int sq = 0; sq < 64; ++sq) {
        int pc = board->Board[sq].type;
        if (pc != EMPTY) {
            gamePhase += gamePhaseInc[pc];
        }
    }
    if (gamePhase > 24) gamePhase = 24;

    int currentDepth = 2;
    for (int i = 0; i < DEPTHS; ++i) {
        if (gamePhase >= depthPhase[i]) {
            currentDepth = depthStage[i];
            break;
        }
    }

    float alpha = -BOARD_INFINITY;
    float beta = BOARD_INFINITY;

    if (board->turn == BLACK_PIECE) {
        *moveEval = MiniMaxAB(board, currentDepth, Min, &alpha, &beta, &botInputStruct->calls[board->moveCount]);
    } else {
        *moveEval = MiniMaxAB(board, currentDepth, Max, &alpha, &beta, &botInputStruct->calls[board->moveCount]);
    }

    botInputStruct->eval = &moveEval->eval;
    *botInputStruct->move = *moveEval->move;

    botInputStruct->calculatedBoardEvaluation[botInputStruct->board->moveCount] = moveEval->eval;
    *botInputStruct->hasMove = 1;
    *botInputStruct->threadRunning = 0;

    free(moveEval);

    pthread_exit(NULL);
}

Move RandomBot(Board *board) {
    // Makes a random move out of all possible moves
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);
    Move move = moves[rand() % movesCount];
    free(moves);

    return move;
}

Move CaptureFirstBot(Board *board) {
    // Prefer captures to other moves
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    Move move;
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);

    int captureCount = 0;
    for (int i = 0; i < movesCount; i++) {
        if (moves[i].moveType == CAPTURE) {
            captureCount++;
        }
    }
    if (captureCount == 0) {
        Move ret = moves[rand() % movesCount];
        free(moves);
        return ret;
    }

    int captureInd = 0;
    int captureWant = rand() % captureCount;
    for (int i = 0; i < movesCount; i++) {
        if (moves[i].moveType == CAPTURE) {
            if (captureInd == captureWant) {
                move = moves[i];
                free(moves);
                return move;
            }
            captureInd++;
        }
    }

    free(moves);

    return move;
}

Move BestScoreBot(Board *board) {
    // Makes every move and finds the board with the best score
    Move *moves = malloc(sizeof (Move) * MAX_MOVES);
    Move move;
    int movesCount = 0;
    GetAllLegalMoves(board, moves, &movesCount);

    float bestScore = -WORST_SCORE;
    if (board->turn == 0) {
        bestScore = WORST_SCORE;
    }

    for (int i = 0; i < movesCount; i++) {
        Board *newBoard = CopyBoard(board);
        UpdateBoard(newBoard, moves[i].pos, moves[i].target, moves[i].moveType);
        CheckStatus(board);
        float score = GetBoardScore(newBoard);

        if (board->turn == 1) {
            if (score >= bestScore) {
                move = moves[i];
                bestScore = score;
            }
        } else {
            if (score <= bestScore) {
                move = moves[i];
                bestScore = score;
            }
        }
    }

    return move;
}