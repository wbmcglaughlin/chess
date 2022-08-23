//
// Created by Will McGlaughlin on 23/8/2022.
//

#include "piecePositionEval.h"

void InitPiecePositionEvalTables() {
    int pc, p, sq;
    for (p = 0, pc = 0; p <= 5; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][sq];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][sq];
            mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
}

int PiecePositionEval(Board *board)
{
    int mg[2];
    int eg[2];
    int gamePhase = 0;

    mg[1] = 0;
    mg[0] = 0;
    eg[1] = 0;
    eg[0] = 0;

    // Evaluate each piece
    for (int sq = 0; sq < 64; ++sq) {
        int pc = board->Board[sq].type;
        if (pc != EMPTY) {
            mg[PCOLOR(pc)] += mg_table[pc][sq];
            eg[PCOLOR(pc)] += eg_table[pc][sq];
            gamePhase += gamePhaseInc[pc];
        }
    }

    /* tapered eval */
    int mgScore = mg[board->turn] - mg[OTHER(board->turn)];
    int egScore = eg[board->turn] - eg[OTHER(board->turn)];
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24;
}