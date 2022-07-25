#include "stdlib.h"
#include "raylib.h"
#include "Game/board.h"
#include "Game/moves.h"
#include "Game/draw.h"
#include "Game/update.h"

#define TARGET_FPS 120

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    BoardDimensions *boardDimensions = malloc(sizeof(BoardDimensions));
    boardDimensions->screenWidth = 700;
    boardDimensions->screenHeight = 500;
    boardDimensions->definingLength = boardDimensions->screenWidth > boardDimensions->screenHeight
                                      ? boardDimensions->screenHeight : boardDimensions->screenWidth;
    boardDimensions->cornerX = (int) (boardDimensions->definingLength * (1 - BOARD_PERCENT) / 2);
    boardDimensions->cornerY = (int) (boardDimensions->definingLength * (1 - BOARD_PERCENT) / 2);
    boardDimensions->sideSize = (int) (boardDimensions->definingLength * BOARD_PERCENT);

    char *fen;
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board *board = CreateBoard();
    if (board == NULL) {
        exit(-1);
    }

    int *moveSquares = malloc(sizeof(int) * SQUARE_COUNT * SQUARE_COUNT);
    ClearMoves(moveSquares);

    int movesCount = 0;
    Move *moves = malloc(sizeof(Move) * MAX_MOVES);

    FenToBoard(fen, board);

    InitWindow(boardDimensions->screenWidth, boardDimensions->screenHeight, "Chess - v.0.2 [Will McGlaughlin]");
    SetTargetFPS(TARGET_FPS);

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
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

    Texture2D *textures[] = {&pw, &pb, &rw, &rb, &nw, &nb, &bw, &bb, &qw, &qb, &kw, &kb};

    // Game Variables
    int selected = -1;
    int pieceHeld = 0;
    int getMoves = 1;

    board->turn = 1;

    int playerWhite = PLAYER;
    int playerBlack = BOT;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        BoardUpdateLoop(board, boardDimensions, moveSquares, &movesCount, moves, &getMoves, &selected, &pieceHeld);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BROWN);
        DrawBoard(boardDimensions, moveSquares, selected);
        DrawPieces(boardDimensions, board, textures, pieceHeld, selected, GetMousePosition());

        DrawText(TextFormat("Score: %.2f", GetBoardScore(board)),
                 (int) (1.2 * boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Moves: %i", movesCount),
                 (int) (1.2 * boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 20,
                 FONT_SIZE,
                 BLACK);

        // ListLegalMoves(moves, movesCount, boardDimensions);
        DrawFPS(5, 5);
        if (board->checkMate) {
            DrawText("Checkmate!", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 20, RED);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Texture unloading
    for (int i = 0; i < UNIQUE_PIECE_TEXTURES; i++) {
        UnloadTexture(*textures[i]);
    }

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    FreeBoard(board);
    free(moves);
    free(moveSquares);
    free(boardDimensions);

    return 0;
}