#include "stdlib.h"
#include "raylib.h"
#include "time.h"
#include "sys/stat.h"

#include "Game/update.h"
#include "Graphics/arrows.h"
#include "Graphics/evaluationBar.h"
#include "Graphics/guiGrid.h"

#define TARGET_FPS 60

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    BoardDimensions *boardDimensions = malloc(sizeof(BoardDimensions));
    boardDimensions->screenWidth = 1200;
    boardDimensions->screenHeight = 720;
    boardDimensions->definingLength = boardDimensions->screenWidth > boardDimensions->screenHeight
                                      ? boardDimensions->screenHeight : boardDimensions->screenWidth;
    boardDimensions->cornerX = (int) (boardDimensions->definingLength * (1 - BOARD_PERCENT) / 2);
    boardDimensions->cornerY = (int) (boardDimensions->definingLength * (1 - BOARD_PERCENT) / 2);
    boardDimensions->sideSize = (int) (boardDimensions->definingLength * BOARD_PERCENT);

    InitWindow(boardDimensions->screenWidth, boardDimensions->screenHeight, "Chess - v.0.3 [Will McGlaughlin]");
    SetTargetFPS(TARGET_FPS);

    struct stat st = {0};

    if (stat("resources/results", &st) == -1) {
        mkdir("resources/results", 0700);
    }

    // Menu
    // -----------------------------------------------------------------------------------------------------------------
    float gridX = 2.0f * (float) boardDimensions->cornerX + (float) boardDimensions->sideSize;
    float gridY = (float) boardDimensions->cornerY;
    int gridRows = 20;
    int gridCols = 4;
    float border = 2.0f;
    Grid *grid = SetGridRectangles(gridX,
                                   gridY,
                                   (float) boardDimensions->screenWidth - gridX - (float) boardDimensions->cornerX,
                                   (float) boardDimensions->sideSize,
                                   gridRows,
                                   gridCols,
                                   border);

    int restartButtonPos = gridRows - 1;
    int statusButtonPos = gridRows - 2;
    int playerSelectionRowPos = gridRows - 3;

    static Color backgroundColor = NIGHTBLUE;

    int isGameRunning = 0;
    char statusText[2][6];
    strcpy(statusText[0], "Start");
    strcpy(statusText[1], "Stop");

    // Evaluation Bar
    float evalBarPercentage = 0.8f;
    Rectangle evalBarRec = (Rectangle) {(float) boardDimensions->cornerX - (float) boardDimensions->cornerX * evalBarPercentage,
                                        (float) boardDimensions->cornerY,
                                        (float) boardDimensions->cornerX * (2 * evalBarPercentage - 1),
                                        (float) boardDimensions->sideSize};

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
    Texture2D pw = LoadTexture("resources/pieces/pw.png");
    Texture2D pb = LoadTexture("resources/pieces/pb.png");
    Texture2D nw = LoadTexture("resources/pieces/nw.png");
    Texture2D nb = LoadTexture("resources/pieces/nb.png");
    Texture2D bw = LoadTexture("resources/pieces/bw.png");
    Texture2D bb = LoadTexture("resources/pieces/bb.png");
    Texture2D rw = LoadTexture("resources/pieces/rw.png");
    Texture2D rb = LoadTexture("resources/pieces/rb.png");
    Texture2D qw = LoadTexture("resources/pieces/qw.png");
    Texture2D qb = LoadTexture("resources/pieces/qb.png");
    Texture2D kw = LoadTexture("resources/pieces/kw.png");
    Texture2D kb = LoadTexture("resources/pieces/kb.png");

    Texture2D *textures[] = {&pw, &pb, &nw, &nb, &bw, &bb, &rw, &rb, &qw, &qb, &kw, &kb};

    char *fen;
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    int players[2] = {0, 0};
    Game *gameInstance = NewGameInstanceFromFen(fen, players);

    Vector2 *mousePosition = malloc(sizeof (Vector2));

    // Piece Position Array Loading to Memory
    InitPiecePositionEvalTables();

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
        *mousePosition = GetMousePosition();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(backgroundColor);
        DrawBoard(boardDimensions, gameInstance->moveSquares, selected);
        DrawPieces(boardDimensions, gameInstance->board, textures, pieceHeld, selected, GetMousePosition());

        int recPos = playerSelectionRowPos * gridCols;
        if (CheckCollisionPointRec(*mousePosition, grid->recs[recPos + 1])) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                players[0] = (players[0] + 1) % 2;
            }
        }

        if (CheckCollisionPointRec(*mousePosition, grid->recs[recPos + 3])) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                players[1] = (players[1] + 1) % 2;
            }
        }

        // Restart Game
        if (CheckCollisionPointRec(*mousePosition, grid->rowRecs[restartButtonPos])) {
            // Wait for thread to finish, then create new game
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int wait_time = 1;

                while (*gameInstance->botInput.threadRunning == 1) {
                    wait(&wait_time);
                    printf("\rWaiting for thread to finish!");
                }

                gameInstance = NewGameInstanceFromFen(fen, players);
            }
        }

        // Start Stop Game
        if (CheckCollisionPointRec(*mousePosition, grid->rowRecs[statusButtonPos])) {
            // Wait for thread to finish, then create new game
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                isGameRunning = (isGameRunning + 1) % 2;
            }
        }

        // Update
        //----------------------------------------------------------------------------------
        if (isGameRunning) {
            BoardUpdateLoop(gameInstance,
                            boardDimensions,
                            &getMoves,
                            &selected,
                            &pieceHeld);
        }

        DrawGameInstanceInfo(gameInstance, boardDimensions, RAYWHITE);
        DrawEvaluationBar(&evalBarRec, &gameInstance->eval);

        // Player Type Selection
        for (int i = 0; i < 2; ++i) {
            DrawRectangleRec(grid->recs[recPos + 2 * i], RAYWHITE);

            DrawText(TextFormat("P %i:", i),
                     (int) (grid->recs[recPos + 2 * i].x + 5.0f),
                     (int) (grid->recs[recPos + 2 * i].y + 5.0f),
                     (int) (grid->recs[recPos + 2 * i].height / 1.1f),
                     NIGHTBLUE);

            DrawRectangleRec(grid->recs[recPos + 2 * i + 1], RAYWHITE);

            DrawText(TextFormat("%i", players[i]),
                     (int) (grid->recs[recPos + 2 * i + 1].x + 5.0f),
                     (int) (grid->recs[recPos + 2 * i + 1].y + 5.0f),
                     (int) (grid->recs[recPos + 2 * i + 1].height / 1.1f),
                     NIGHTBLUE);
        }

        // Restart Game Button Drawing
        DrawRectangleRec(grid->rowRecs[restartButtonPos], RAYWHITE);
        DrawText("Restart",
                 (int) (grid->rowRecs[restartButtonPos].x + 5.0f),
                 (int) (grid->rowRecs[restartButtonPos].y + 5.0f),
                 (int) (grid->rowRecs[restartButtonPos].height / 1.1f),
                 NIGHTBLUE);

        // Start Stop Button
        DrawRectangleRec(grid->rowRecs[statusButtonPos], RAYWHITE);
        DrawText(statusText[isGameRunning],
                 (int) (grid->rowRecs[statusButtonPos].x + 5.0f),
                 (int) (grid->rowRecs[statusButtonPos].y + 5.0f),
                 (int) (grid->rowRecs[statusButtonPos].height / 1.1f),
                 NIGHTBLUE);

        DrawArrows(&firstArrow, &squarePressed, &squareReleased, mousePosition, boardDimensions);

        if (gameInstance->board->checkMate) {
            DrawText("Checkmate!", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 40, RED);
        }

        if (gameInstance->board->draw) {
            DrawText("Draw :(", boardDimensions->screenWidth / 2, boardDimensions->screenHeight / 2, 40, RED);
        }

        // DrawSquareValues(boardDimensions, gameInstance->board);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    FILE *fptr;

    fptr = fopen("resources/results/game.csv", "w");
    fprintf(fptr, "move, calls, eval \n");
    for (int i = 0; i < gameInstance->board->moveCount; i++) {
        fprintf(fptr, "%i, %i, %.4f\n",
                i,
                gameInstance->botInput.calls[i],
                gameInstance->botInput.calculatedBoardEvaluation[i]);
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
    int wait_time = 1;

    while (*gameInstance->botInput.threadRunning == 1) {
        wait(&wait_time);
        printf("\rWaiting for thread to finish!");
    }

    FreeGameInstance(gameInstance);
    free(boardDimensions);
    free(mousePosition);

    return 0;
}