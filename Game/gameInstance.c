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

    game->board->turn = 1;
    game->players[0] = BOT;
    game->players[1] = BOT;

    game->botInput = *CreateBotInput(board);
    game->botInput.calls = malloc(sizeof (int ) * MAX_MOVES);

    return game;
}

void FreeGameInstance(Game *game) {
    FreeBoard(game->board);
    free(game->botInput.calls);
    free(game->moves);
    free(game->moveSquares);
    free(game);
}

