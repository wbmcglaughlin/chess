#include "stdlib.h"
#include "raylib.h"
#include "time.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "Game/board.h"
#include "Game/moves.h"
#include "Game/draw.h"
#include "Game/update.h"
#include "Game/moveList.h"
#include "Game/gameInstance.h"

#define TARGET_FPS 60

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

    InitWindow(boardDimensions->screenWidth, boardDimensions->screenHeight, "Chess - v.0.2 [Will McGlaughlin]");
    SetTargetFPS(TARGET_FPS);

    struct stat st = {0};

    if (stat("resources/Results", &st) == -1) {
        mkdir("resources/Results", 0700);
    }

    // Menu
    // -----------------------------------------------------------------------------------------------------------------
    static Color backgroundColor = (Color) {120, 120, 120, 255};

    Vector2 restartButtonCorner = (Vector2) {2.0f * boardDimensions->cornerX + boardDimensions->sideSize,
                                             (float) boardDimensions->cornerY + (float) boardDimensions->sideSize - 100.0f};
    Rectangle restartButtonRec = (Rectangle) {restartButtonCorner.x,
                                              restartButtonCorner.y,
                                              160.0f,
                                              40.0f};

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

    char *fen;
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Game *gameInstance = NewGameInstanceFromFen(fen);

    // Game Variables
    int selected = -1;
    int pieceHeld = 0;
    int getMoves = 1;

    // Player Drawn Arrows
    MoveNodePtr firstArrow = NULL;
    int squarePressed = -1;
    int squareReleased = -1;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(backgroundColor);
        DrawBoard(boardDimensions, gameInstance->moveSquares, selected);
        DrawPieces(boardDimensions, gameInstance->board, textures, pieceHeld, selected, GetMousePosition());

        if (CheckCollisionPointRec(GetMousePosition(), restartButtonRec)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                FenToBoard(fen, gameInstance->board);
            }
        }

        // Update
        //----------------------------------------------------------------------------------
        BoardUpdateLoop(gameInstance,
                        boardDimensions,
                        &getMoves,
                        &selected,
                        &pieceHeld);

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            GetSelected(&squarePressed, GetMousePosition().x, GetMousePosition().y, boardDimensions);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!IsMoveNodeListEmpty(firstArrow)) {
                while (firstArrow->next != NULL) {
                    DeleteMoveNode(&firstArrow);
                }
                DeleteMoveNode(&firstArrow);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            GetSelected(&squareReleased, GetMousePosition().x, GetMousePosition().y, boardDimensions);

            if (squarePressed != -1 && squareReleased != -1) {
                Move arrowMove = (Move) {squarePressed, squareReleased, 0};
                InsertMoveNode(&firstArrow, arrowMove);
            }

            squarePressed = -1;
            squareReleased = -1;
        }

        if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            squarePressed = -1;
        }

        DrawText(TextFormat("Score: %.2f", gameInstance->board->eval),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Moves: %i", gameInstance->movesCount),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 20,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Calls: %i", gameInstance->botInput.calls[gameInstance->board->moveCount]),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 40,
                 FONT_SIZE,
                 BLACK);

        DrawText(TextFormat("Half Move Clock: %i", gameInstance->board->halfMoveClock),
                 (int) (boardDimensions->cornerX * 2 + boardDimensions->sideSize),
                 boardDimensions->cornerY + 60,
                 FONT_SIZE,
                 BLACK);

        // Restart Game Button Drawing
        DrawRectangleRec(restartButtonRec, GRAY);
        DrawText("Restart", restartButtonCorner.x + 5.0f, restartButtonCorner.y + 5.0f, restartButtonRec.height / 1.1f, DARKGRAY);

        MoveNodePtr startPtr = firstArrow;
        if (!IsMoveNodeListEmpty(firstArrow)) {
            DrawArrow(startPtr->move.pos, startPtr->move.target, BLUE, boardDimensions);
            while (startPtr->next != NULL) {
                startPtr = startPtr->next;
                DrawArrow(startPtr->move.pos, startPtr->move.target, BLUE, boardDimensions);
            }
        }

        if (gameInstance->board->checkMate) {
            DrawText("Checkmate!", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 20, RED);
        }

        if (gameInstance->board->draw) {
            DrawText("Draw :(", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 20, RED);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    FILE *fptr;

    fptr = fopen("resources/Results/game_sorted.csv", "w");
    fprintf(fptr, "move, calls, \n");
    for (int i = 0; i < gameInstance->board->moveCount; i++) {
        fprintf(fptr, "%i, %i\n", i, gameInstance->botInput.calls[i]);
    }

    fclose(fptr);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Texture unloading
    for (int i = 0; i < UNIQUE_PIECE_TEXTURES; i++) {
        UnloadTexture(*textures[i]);
    }

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    FreeGameInstance(gameInstance);
    free(boardDimensions);

    return 0;
}