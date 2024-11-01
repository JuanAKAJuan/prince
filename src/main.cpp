#include "raylib.h"
#include "rlgl.h"

typedef enum { IDLE, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN } PlayerState;

int main() {
    const int SCREEN_WIDTH{1280};
    const int SCREEN_HEIGHT{800};

    const int PLAYER_SPRITE_COLUMNS{6};
    const int PLAYER_SPRITE_ROWS{10};

    const float FRAME_TIME{0.1f};

    const int MAP_ROWS{10};
    const int MAP_COLS{10};
    const int TILE_SIZE{32};
    int tileMap[MAP_ROWS][MAP_COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 1, 1, 1, 1}, {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 0, 1, 1, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
        {0, 1, 1, 1, 0, 0, 0, 0, 1, 1}, {1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
    };

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Prince");

    Texture2D characterSheet = LoadTexture("resources/Player/Player.png");
    Texture2D tileTextures[2] = {0};
    tileTextures[0] = LoadTexture("resources/Tiles/Grass_Middle.png");
    tileTextures[1] = LoadTexture("resources/Tiles/Water_Middle.png");

    int frameWidth = characterSheet.width / PLAYER_SPRITE_COLUMNS;
    int frameHeight = characterSheet.height / PLAYER_SPRITE_ROWS;

    PlayerState playerState{IDLE};
    Vector2 playerPosition = {MAP_COLS * TILE_SIZE / 2.0f,
                              MAP_ROWS * TILE_SIZE / 2.0f};

    int currentFrame{0};
    float frameCounter{0.0f};
    Rectangle frameRectangle = {0.0f, 0.0f, static_cast<float>(frameWidth),
                                static_cast<float>(frameHeight)};

    Camera2D camera = {0};
    camera.target = playerPosition;
    camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
    camera.zoom = 1.0f;

    float moveSpeed = TILE_SIZE * 0.1f;
    float zoomSpeed = 1.0f;
    float minimumZoom = 5.0f;
    float maximumZoom = 30.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 playerMovement = {0.0f, 0.0f};
        bool isMoving = false;

        if (IsKeyDown(KEY_W)) {
            playerMovement.y -= moveSpeed;
            playerState = WALK_UP;
            isMoving = true;
        }
        if (IsKeyDown(KEY_S)) {
            playerMovement.y += moveSpeed;
            playerState = WALK_DOWN;
            isMoving = true;
        }
        if (IsKeyDown(KEY_A)) {
            playerMovement.x -= moveSpeed;
            playerState = WALK_LEFT;
            isMoving = true;
        }
        if (IsKeyDown(KEY_D)) {
            playerMovement.x += moveSpeed;
            playerState = WALK_RIGHT;
            isMoving = true;
        }

        if (!isMoving)
            playerState = IDLE;

        playerPosition.x += playerMovement.x;
        playerPosition.y += playerMovement.y;

        float wheelMove = GetMouseWheelMove();
        camera.zoom += wheelMove * zoomSpeed;

        if (camera.zoom < minimumZoom)
            camera.zoom = minimumZoom;
        if (camera.zoom > maximumZoom)
            camera.zoom = maximumZoom;

        camera.target = playerPosition;
        camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};

        // Frame animation logic
        frameCounter += GetFrameTime();
        if (frameCounter >= FRAME_TIME) {
            frameCounter = 0.0f;
            currentFrame++;

            // Loop back to the first frame
            if (currentFrame >= PLAYER_SPRITE_COLUMNS)
                currentFrame = 0;
        }

        frameRectangle.x = static_cast<float>(currentFrame * frameWidth);
        frameRectangle.width = static_cast<float>(frameWidth);

        switch (playerState) {
            case IDLE:
                frameRectangle.y = 0.0f; // 1st row
                break;
            case WALK_DOWN:
                frameRectangle.y =
                    static_cast<float>(frameHeight * 3); // 4th row
                break;
            case WALK_UP:
                frameRectangle.y =
                    static_cast<float>(frameHeight * 5); // 6th row
                break;
            case WALK_LEFT:
                frameRectangle.y =
                    static_cast<float>(frameHeight * 4); // 5th row
                frameRectangle.width = -frameRectangle.width;
                frameRectangle.x += frameWidth;
                break;
            case WALK_RIGHT:
                frameRectangle.y =
                    static_cast<float>(frameHeight * 4); // 5th row
                break;
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        BeginMode2D(camera);
        for (int y = 0; y < MAP_ROWS; y++) {
            for (int x = 0; x < MAP_COLS; x++) {
                int tileType = tileMap[y][x];
                Vector2 position = {static_cast<float>(x * TILE_SIZE),
                                    static_cast<float>(y * TILE_SIZE)};
                DrawTextureEx(tileTextures[tileType], position, 0.0f,
                              static_cast<float>(TILE_SIZE) /
                                  tileTextures[tileType].width,
                              WHITE);
            }
        }

        float playerScale = 2.0f;
        Rectangle destinationRectangle = {playerPosition.x, playerPosition.y,
                                          frameWidth * playerScale,
                                          frameHeight * playerScale};

        Vector2 origin = {frameWidth * playerScale / 2.0f,
                          frameHeight * playerScale / 2.0f};

        DrawTexturePro(characterSheet, frameRectangle, destinationRectangle,
                       origin, 0.0f, WHITE);

        EndMode2D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(characterSheet);
    UnloadTexture(tileTextures[0]);
    UnloadTexture(tileTextures[1]);
    CloseWindow();
    return 0;
}
