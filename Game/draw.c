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

void DrawPieces(BoardDimensions *boardDimensions, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, boardDimensions->cornerX, boardDimensions->cornerY);
        char type = board->Board[i].type;
        if (type != 'e') {
            for (int j = 0; j < UNIQUE_PIECE_TEXTURES; j++) {
                if (type == PIECES[j]) {
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

void DrawBoardInfo(Board *board, BoardDimensions *boardDimensions) {
    int moves = GetAllMovesCount(board);
    DrawText(TextFormat("Moves: %i", moves),
             (int) (1.2 * boardDimensions->cornerX + boardDimensions->sideSize),
             boardDimensions->cornerY,
             FONT_SIZE,
             BLACK);

    DrawText(TextFormat("Castle: K: %i Q: %i k: %i q: %i",
                        board->castle[0],
                        board->castle[1],
                        board->castle[2],
                        board->castle[3]),
             (int) (1.2 * boardDimensions->cornerX + boardDimensions->sideSize),
             boardDimensions->cornerY + FONT_SIZE * 1,
             FONT_SIZE,
             BLACK);

    DrawText(TextFormat("Turn: %i", board->turn),
             (int) (1.2 * boardDimensions->cornerX + boardDimensions->sideSize),
             boardDimensions->cornerY + FONT_SIZE * 2,
             FONT_SIZE,
             BLACK);
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

    DrawLineEx(s, e, 5.0f, GREEN);

    float angle = 0.0f;
    float perpAngle = 0.0f;
    if (r_e != r_s && c_e != c_s) {
        angle = atanf((e.y - s.y) / (e.x - s.x));
        perpAngle = atanf(-(e.x - s.x) / (e.y - s.y));
    }
    if (c_e == c_s) {
        angle = PI / 2;
        perpAngle = 0.0f;
    }
    if (r_e == r_s) {
        angle = 0.0f;
        perpAngle = PI / 2;
    }

    if (c_e < c_s) {
        angle = 180 - angle;
        perpAngle = 180 - perpAngle;
    }

    float size = 17.0f;
    Vector2 bl = (Vector2) {e.x + size * cosf(perpAngle), e.y + size * sinf(perpAngle)};
    Vector2 br = (Vector2) {e.x - size * cosf(perpAngle), e.y - size * sinf(perpAngle)};
    Vector2 t = (Vector2) {e.x + size * cosf(angle), e.y + size * sinf(angle)};

    if (c_e > c_s) {
        DrawTriangle(bl, br, t, GREEN);
    } else {
        DrawTriangle(br, bl, t, GREEN);
    }

}
