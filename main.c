#include "stdlib.h"

#include "raylib.h"

#include "board.h"

#define TARGET_FPS 244

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1000;
    int screenHeight = 800;

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

    int *moves = malloc(sizeof (int) * SQUARE_COUNT * SQUARE_COUNT);
    ClearMoves(moves);

    FenToBoard(fen, board);

    InitWindow(screenWidth, screenHeight, "Chess - v.0.1 [Will McGlaughlin]");
    SetTargetFPS(TARGET_FPS);

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D pw = LoadTexture("resources/pieces/pw.png");
    Texture2D pb = LoadTexture("resources/pieces/pb.png");
    Texture2D rw = LoadTexture("resources/pieces/rw.png");
    Texture2D rb = LoadTexture("resources/pieces/rb.png");
    Texture2D nw = LoadTexture("resources/pieces/nw.png");
    Texture2D nb = LoadTexture("resources/pieces/nb.png");
    Texture2D bw = LoadTexture("resources/pieces/bw.png");
    Texture2D bb = LoadTexture("resources/pieces/bb.png");
    Texture2D qw = LoadTexture("resources/pieces/qw.png");
    Texture2D qb = LoadTexture("resources/pieces/qb.png");
    Texture2D kw = LoadTexture("resources/pieces/kw.png");
    Texture2D kb = LoadTexture("resources/pieces/kb.png");

    Texture2D *textures[] = {&pw, &pb, &rw, &rb, &nw, &nb, &bw, &bb,&qw, &qb, &kw, &kb};

    // Game Variables
    int selected = -1;
    int pieceHeld = 0;
    int getMoves = 1;

    board->turn = 1;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (selected != -1) {
                pieceHeld = 1;
            } else {
                GetSelected(&selected,
                            GetMousePosition().x,
                            GetMousePosition().y,
                            borderSize,
                            borderSize,
                            boardSideSize);
                if (board->Board[selected].color == board->turn) {
                    GetMoves(board, moves, selected);
                }
                getMoves = (getMoves + 1) % 2;
                if (board->Board[selected].color != board->turn) {
                    selected = -1;
                }
            }
        } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) && pieceHeld) {
            int pieceSquare = selected;
            GetSelected(&selected,
                        GetMousePosition().x,
                        GetMousePosition().y,
                        borderSize,
                        borderSize,
                        boardSideSize);

            // If valid square and not the same square
            if (selected != -1 && selected != pieceSquare && moves[selected] > 0) {
                UpdateBoard(board, pieceSquare, selected, moves[selected]);
            }
            pieceHeld = 0;
            getMoves = (getMoves + 1) % 2;
            ClearMoves(moves);
            selected = -1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawBoard(borderSize, borderSize, boardSideSize, moves, selected);
        DrawPieces(borderSize,
                   borderSize,
                   boardSideSize,
                   board,
                   textures,
                   pieceHeld,
                   selected,
                   GetMousePosition());

        DrawFPS(5, 5);

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