#include "raylib.h"
#include "board.h"
#include "stdlib.h"

#define BOARD_PERCENT .9

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 600;
    int screenHeight = 400;

    int definingLength = screenWidth;
    if (screenWidth > screenHeight) {
        definingLength = screenHeight;
    }

    int boardSideSize = (int) (definingLength * BOARD_PERCENT);
    int borderSize = (int) (definingLength * (1 - BOARD_PERCENT) / 2);

    char *fen;
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board *board = malloc(sizeof (*board));
    board->Board = malloc(sizeof (Piece) * 64);

    FenToBoard(fen, board);

    InitWindow(screenWidth, screenHeight, "Chess - v.0.1 [Will McGlaughlin]");
    SetTargetFPS(244);

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D pw = LoadTexture("resources/pieces/pw.png");
    Texture2D pb = LoadTexture("resources/pieces/pb.png");
    Texture2D rw = LoadTexture("resources/pieces/rw.png");
    Texture2D rb = LoadTexture("resources/pieces/rb.png");
    Texture2D nw = LoadTexture("resources/pieces/nw.png");
    Texture2D nb = LoadTexture("resources/pieces/nb.png");
    Texture2D bw = LoadTexture("resources/pieces/bw.png");
    Texture2D bb = LoadTexture("resources/pieces/bb.png");
    Texture2D kw = LoadTexture("resources/pieces/kw.png");
    Texture2D kb = LoadTexture("resources/pieces/kb.png");
    Texture2D qw = LoadTexture("resources/pieces/qw.png");
    Texture2D qb = LoadTexture("resources/pieces/qb.png");

    Texture2D *textures[] = {&pw, &pb, &rw, &rb, &nw, &nb, &bw, &bb, &kw, &kb, &qw, &qb};

    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawBoard(borderSize, borderSize, boardSideSize);
        DrawPieces(borderSize, borderSize, boardSideSize, board, textures);

        // DrawFPS(5, 5);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < UNIQUE_PIECE_TEXTURES; i++) {
        UnloadTexture(*textures[i]);
    }
    // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}