//
// Created by Will McGlaughlin on 23/8/2022.
//

#include "piecePositionEval.h"

void InitPiecePositionEvalTables() {
    int pc, p, sq;
    for (p = PAWN, pc = WHITE_PAWN; p <= KING; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            // Values are added to the table, given tables are flipped to
            // be in whites orientation, so we have to flip the square index.
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
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

    int turn = board->turn;

    // Assign each piece the respective middle game and end game weighting.
    for (int sq = 0; sq < 64; ++sq) {
        int pc = board->Board[sq].type;
        if (pc != EMPTY) {
            mg[PCOLOR(pc)] += mg_table[pc][sq];
            eg[PCOLOR(pc)] += eg_table[pc][sq];
            gamePhase += gamePhaseInc[pc];
        }
    }

    // Comparing the eval of the current players value to the
    // opponents value.
    int mgScore = mg[turn] - mg[OTHER(turn)];
    int egScore = eg[turn] - eg[OTHER(turn)];

    // We want to weight the analysis towards the estimated game phase.
    // For example, if gamePhase is high (eg. > 12), the mgPhase variable
    // will be higher than the egPhase, and the returned weighting will be
    // middle game oriented.
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;

    // Return the negative equivalent of the evaluation for blacks turn
    return (1 - 2 * turn) * (mgScore * mgPhase + egScore * egPhase) / 24;
}