//
// Created by wbmcg on 15/05/2022.
//

#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include "../Game/board.h"
#include "../Game/moves.h"
#include "math.h"

Move RandomBot(Board *board);
Move CaptureFirstBot(Board *board);

#endif //CHESS_BOT_H
