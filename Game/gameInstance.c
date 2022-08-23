//
// Created by Will McGlaughlin on 18/8/2022.
//

#include "gameInstance.h"

Game *NewGameInstanceFromFen(char* fen) {
    Board *board = CreateBoard();
    if (board == NULL) {
        exit(-1);
    }

    Game *game = malloc(sizeof (*board) + 2 * sizeof (BotInput));

    game->board = board;

    FenToBoard(fen, game->board);

    game->moveSquares = malloc(sizeof(int) * SQUARE_COUNT * SQUARE_COUNT);
    ClearMoves(game->moveSquares);

    game->movesCount = 0;

    game->moves = malloc(sizeof(Move) * MAX_MOVES);

    game->players[WHITE_PIECE] = BOT;
    game->players[BLACK_PIECE] = BOT;

    game->botInput = *CreateBotInput(board);
    game->botInput.calls = malloc(sizeof (int ) * MAX_MOVES);

    game->eval = 0.0f;

    return game;
}

void FreeGameInstance(Game *game) {
    FreeBoard(game->board);
    free(game->botInput.calls);
    FreeBotInput(&game->botInput);
    free(game->moves);
    free(game->moveSquares);
    free(game);
}

void DrawGameInstanceInfo(Game *gameInstance, BoardDimensions *boardDimensions) {
    DrawText(TextFormat("Score: %.2f", gameInstance->eval),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             boardDimensions->cornerY,
             FONT_SIZE,
             BLACK);

    DrawText(TextFormat("Moves: %i", gameInstance->movesCount),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             boardDimensions->cornerY + 20,
             FONT_SIZE,
             BLACK);

    DrawText(TextFormat("Calls: %i", gameInstance->botInput.calls[gameInstance->board->moveCount]),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             boardDimensions->cornerY + 40,
             FONT_SIZE,
             BLACK);

    DrawText(TextFormat("Half Move Clock: %i", gameInstance->board->halfMoveClock),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             boardDimensions->cornerY + 60,
             FONT_SIZE,
             BLACK);
}

