//
// Created by Will McGlaughlin on 13/5/2022.
//

#include "draw.h"

void DrawBoard(BoardDimensions *boardDimensions, const int *moves, int selected) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, boardDimensions->cornerX, boardDimensions->cornerY);
        DrawRectangle(boardSquare->cornerX,
                      boardSquare->cornerY,
                      squareWidth,
                      squareWidth,
                      ((boardSquare->x + boardSquare->y) % 2 == 0) ? DARKGRAY : RAYWHITE
        );

        DrawText(TextFormat("%i", i),
                 boardSquare->cornerX + 2,
                 boardSquare->cornerY + 2,
                 10, DARKBLUE);

        if (i == selected) {
            DrawText(TextFormat("Selected: %i", i), 400, 5, 20, BLACK);
            DrawRectangle(boardSquare->cornerX,
                          boardSquare->cornerY,
                          squareWidth,
                          squareWidth,
                          GREEN
            );
        }
        if (moves[i] > 0) {
            DrawRectangle(boardSquare->cornerX,
                          boardSquare->cornerY,
                          squareWidth,
                          squareWidth,
                          DARKGREEN
            );
        }
        free(boardSquare);
    }
}

void DrawSquareValues(BoardDimensions *boardDimensions, Board *board) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, boardDimensions->cornerX, boardDimensions->cornerY);
        DrawText(TextFormat("%.2f", board->Board[i].score),
                  boardSquare->cornerX + 2,
                  boardSquare->cornerY + 2,
                  10, DARKBLUE);
        free(boardSquare);
    }
}

void DrawPieces(BoardDimensions *boardDimensions, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, boardDimensions->cornerX, boardDimensions->cornerY);
        int type = board->Board[i].type;
        if (type != EMPTY) {
            for (int j = 0; j < UNIQUE_PIECE_TEXTURES; j++) {
                if (type == j) {
                    if (pieceHeld && i == selected) {
                        DrawTextureEx(*pieceTextures[j],
                                      (Vector2) {mousePosition.x - (float) squareWidth / 2, mousePosition.y - (float) squareWidth / 2},
                                      0,
                                      (float) squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    } else {
                        DrawTextureEx(*pieceTextures[j],
                                      (Vector2) {(float) boardSquare->cornerX, (float) boardSquare->cornerY},
                                      0,
                                      (float) squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    }
                }
            }
        }
        free(boardSquare);
    }
}

void ListLegalMoves(Move *moves, int movesCount, BoardDimensions *boardDimensions) {
    for (int i = 0; i < movesCount; i++) {
        DrawText(TextFormat("%i %i", moves[i].pos, moves[i].target),
                 (int) (1.2 * boardDimensions->cornerX + boardDimensions->sideSize),
                 boardDimensions->cornerY + FONT_SIZE / 2 * i,
                 FONT_SIZE / 2,
                 BLACK);
    }
}

void DrawArrow(int startSquare, int endSquare, Color color, BoardDimensions *bd) {
    float arrowSize = 17.0f;

    float squareSideDim = (float) (bd->sideSize) / SQUARE_COUNT;

    int r_s = (int) startSquare / SQUARE_COUNT;
    int c_s = (int) startSquare % SQUARE_COUNT;

    int r_e = (int) endSquare / SQUARE_COUNT;
    int c_e = (int) endSquare % SQUARE_COUNT;

    Vector2 s = (Vector2) {
        (float) c_s * squareSideDim + (float) bd->cornerX + squareSideDim / 2.0f,
        (float) -r_s * squareSideDim + (float) bd->cornerY - squareSideDim / 2.0f + (float) bd->sideSize};
    Vector2 e = (Vector2) {
        (float) c_e * squareSideDim + (float) bd->cornerX + squareSideDim / 2.0f,
        (float) -r_e * squareSideDim + (float) bd->cornerY - squareSideDim / 2.0f + (float) bd->sideSize};

    DrawLineEx(s, e, 12.0f, color);

    float angle;

    float rise = e.y - s.y;
    float run = e.x - s.x;

    if (rise != 0 && run != 0) {
        angle = atanf((e.y - s.y) / (e.x - s.x));
    }

    if (rise == 0) {
        angle = 0.0f;
    }

    if (run == 0) {
        angle = PI / 2;
    }

    float perpAngle = angle + PI / 2;
    if (c_e < c_s) {
        angle = PI - angle;
        perpAngle += PI;
    }

    if (r_e > r_s && run == 0 ) {
        perpAngle -= PI;
    }

    DrawPoly(e, 3, arrowSize, (perpAngle + PI) * 180 / PI, color);

}
