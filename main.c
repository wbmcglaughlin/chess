#include "stdlib.h"
#include "raylib.h"
#include "time.h"
#include "Game/board.h"
#include "Game/moves.h"
#include "Game/draw.h"
#include "Game/update.h"

#define TARGET_FPS 60
#define MAX_TURNS 5000

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

    static Color backgroundColor = (Color) {120, 120, 120, 255};

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
    int players[2] = {BOT, BOT};

    // Bot Information
    int hasMove = 0;
    int threadStarted = 0;

    Move move = (Move) {-1, -1};
    float eval = 0.0f;
    int *calls = malloc(sizeof (int) * MAX_TURNS);
    BotInput botInput = (BotInput) {board, &move, &eval, &hasMove, calls};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(backgroundColor);
        DrawBoard(boardDimensions, moveSquares, selected);
        DrawPieces(boardDimensions, board, textures, pieceHeld, selected, GetMousePosition());

        // Update
        //----------------------------------------------------------------------------------
        BoardUpdateLoop(players,
                        board,
                        boardDimensions,
                        &botInput,
                        &threadStarted,
                        moveSquares,
                        &movesCount,
                        moves,
                        &getMoves,
                        &selected,
                        &pieceHeld);

        DrawText(TextFormat("Score: %.2f", board->eval),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Moves: %i", movesCount),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 20,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Calls: %i", calls[board->moveCount]),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 40,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Half Move Clock: %i", board->halfMoveClock),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 60,
                 FONT_SIZE,
                 BLACK);

        if (board->checkMate) {
            DrawText("Checkmate!", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 20, RED);
        }

        if (board->draw) {
            DrawText("Draw :(", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 20, RED);
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