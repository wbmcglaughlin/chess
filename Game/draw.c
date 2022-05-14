//
// Created by Will McGlaughlin on 13/5/2022.
//

#include "draw.h"

void DrawBoard(BoardDimensions *boardDimensions, int *moves, int selected) {
    int squareWidth = (int) boardDimensions->sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARES; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, boardDimensions->cornerX, boardDimensions->cornerY);
        DrawRectangle(boardSquare->cornerX,
                      boardSquare->cornerY,
                      squareWidth,
                      squareWidth,
                      ((boardSquare->x + boardSquare->y) % 2 == 0) ? DARKGRAY : WHITE
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
                          LIGHTGRAY
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
                                      (Vector2) {mousePosition.x - squareWidth / 2, mousePosition.y - squareWidth / 2},
                                      0,
                                      squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    } else {
                        DrawTextureEx(*pieceTextures[j],
                                      (Vector2) {boardSquare->cornerX, boardSquare->cornerY},
                                      0,
                                      squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    }
                }
            }
            free(boardSquare);
        }
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
