//
// Created by Will McGlaughlin on 27/4/2022.
//

#include "board.h"

void GetSquare(BoardSquare *boardSquare, int i, int squareSize, int cornerX, int cornerY) {
    int x = i % SQUARE_COUNT;
    int y = (int) (i / SQUARE_COUNT);

    boardSquare->x = x;
    boardSquare->y = y;
    boardSquare->sideSize = squareSize;
    boardSquare->cornerX  = cornerX + x * squareSize;
    boardSquare->cornerY = cornerY + (SQUARE_COUNT - y - 1) * squareSize;
}

void GetSelected(int *selected, float mousePosX, float mousePosY, int cornerX, int cornerY, int sideSize) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;

    // mousePosX = cornerX + x * squareWidth
    // mousePosY = cornerY + (SQUARE_COUNT - y - 1) * squareWidth;
    int x = (int) ((mousePosX - cornerX) / squareWidth);
    int y = SQUARE_COUNT - (int) ((mousePosY - cornerY) / squareWidth) - 1;

    if (x >= 0 && x < SQUARE_COUNT && y >= 0 && y < SQUARE_COUNT) {
        *selected = x + SQUARE_COUNT * y;
    }
}

void UpdateBoard(Board *board, int pieceSquare, int selected) {
    Piece *piece = malloc(sizeof (Piece));
    *piece = board->Board[pieceSquare];
    board->Board[pieceSquare] = (Piece) {'e', pieceSquare, 0};
    board->Board[selected] = *piece;
}


void DrawBoard(int cornerX, int cornerY, int sideSize, int selected) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARE_COUNT * SQUARE_COUNT; i++) {
        BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
        GetSquare(boardSquare, i, squareWidth, cornerX, cornerY);
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
    }
}

void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12], int pieceHeld, int selected, Vector2 mousePosition) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int i = 0; i < SQUARE_COUNT * SQUARE_COUNT; i++) {
            BoardSquare *boardSquare = malloc(sizeof (BoardSquare));
            GetSquare(boardSquare, i, squareWidth, cornerX, cornerY);
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
        }
    }
}

void FenToBoard(const char *fen, Board *board) {
    int pos = 0;
    for (int c = 0; c < MAX_FEN_LENGTH; c++) {
        char ch = fen[c];
        if (pos < 64) {
            if (strchr(PIECES, ch) != NULL) {
                board->Board[pos] = (Piece) {ch, pos, isupper(ch) == 0 ? 0 : 1};
                ++pos;
            } else if (ch != '/') {
                int pos_change = atoi(&ch);
                for (int i = 0; i < pos_change; i++) {
                    board->Board[pos] = (Piece) {'e', pos, 0};
                    ++pos;
                }
            }
        }
    }
}
