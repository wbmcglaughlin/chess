//
// Created by Will McGlaughlin on 18/8/2022.
//

#ifndef CHESS_GAMEINSTANCE_H
#define CHESS_GAMEINSTANCE_H

#include "../Systems/bot.h"

struct Game {
    Board *board;
    Move *moves;
    int *moveSquares;
    int movesCount;
    int moveCount;
    int players[2];
    BotInput botInput;
    float eval;
};

typedef struct Game Game;

void DrawGameInstanceInfo(Game *gameInstance, BoardDimensions *boardDimensions);
Game *NewGameInstanceFromFen(char*fen);
void FreeGameInstance(Game *game);

#endif //CHESS_GAMEINSTANCE_H
