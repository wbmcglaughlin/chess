//
// Created by Will McGlaughlin on 27/4/2022.
//

#include "board.h"

void DrawBoard(int cornerX, int cornerY, int sideSize) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int x = 0; x < SQUARE_COUNT; x++) {
        for (int y = 0; y < SQUARE_COUNT; y++) {
            DrawRectangle(cornerX + x * squareWidth,
                          cornerY + y * squareWidth,
                          squareWidth,
                          squareWidth,
                          ((y + x) % 2 == 0) ? DARKGRAY : WHITE
                          );
        }
    }
}

void DrawPieces(int cornerX, int cornerY, int sideSize, Board *board, Texture2D *pieceTextures[12]) {
    int squareWidth = (int) sideSize / SQUARE_COUNT;
    for (int x = 0; x < SQUARE_COUNT; x++) {
        for (int y = 0; y < SQUARE_COUNT; y++) {
            char type = board->Board[SQUARE_COUNT * SQUARE_COUNT - (x + SQUARE_COUNT * y) - 1].type;
            if (type != 'e') {
                for (int i = 0; i < UNIQUE_PIECE_TEXTURES; i++) {
                    if (type == PIECES[i]) {
                        DrawTextureEx(*pieceTextures[i],
                                      (Vector2) {cornerX + x * squareWidth, cornerY + y * squareWidth},
                                      0,
                                      squareWidth / (float) (*pieceTextures[0]).height,
                                      WHITE);
                    }
                }
            }
        }
    }
}

void FenToBoard(char *fen, Board *board) {
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
