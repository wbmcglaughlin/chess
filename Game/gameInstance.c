//
// Created by Will McGlaughlin on 18/8/2022.
//

#include "gameInstance.h"

Game *NewGameInstanceFromFen(char* fen, int playerTypes[2]) {
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

    game->players[WHITE_PIECE] = playerTypes[WHITE_PIECE];
    game->players[BLACK_PIECE] = playerTypes[BLACK_PIECE];

    game->botInput = *CreateBotInput(board);
    game->botInput.calls = malloc(sizeof (int ) * MAX_MOVES);

    game->botInput.calculatedBoardEvaluation = malloc(sizeof (float ) * MAX_MOVES);
    game->eval = 0.0f;

    return game;
}

void FreeGameInstance(Game *game) {
    FreeBoard(game->board);
    free(game->botInput.calls);
    free(game->botInput.calculatedBoardEvaluation);
    FreeBotInput(&game->botInput);
    free(game->moves);
    free(game->moveSquares);
    free(game);
}

void DrawGameInstanceInfo(Game *gameInstance, BoardDimensions *boardDimensions, Color color) {
    int i = 0;

    DrawText(TextFormat("Turn: %i", gameInstance->board->turn),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             (int) boardDimensions->cornerY + i * 20,
             FONT_SIZE,
             color);
    ++i;

    DrawText(TextFormat("Score: %.2f", gameInstance->eval),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             (int) boardDimensions->cornerY + i * 20,
             FONT_SIZE,
             color);
    ++i;

    DrawText(TextFormat("Moves: %i", gameInstance->movesCount),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             (int) boardDimensions->cornerY + i * 20,
             FONT_SIZE,
             color);
    ++i;

    DrawText(TextFormat("Calls: %i", gameInstance->botInput.calls[gameInstance->board->moveCount]),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             (int) boardDimensions->cornerY + i * 20,
             FONT_SIZE,
             color);
    ++i;

    DrawText(TextFormat("Half Move: %i", gameInstance->board->halfMoveClock),
             (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
             (int) boardDimensions->cornerY + i * 20,
             FONT_SIZE,
             color);
    ++i;
}

