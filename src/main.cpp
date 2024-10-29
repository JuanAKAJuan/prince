#include "raylib.h"
#include "rlgl.h"

typedef enum { IDLE, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN } PlayerState;

int main() {
    const int SCREEN_WIDTH{1280};
    const int SCREEN_HEIGHT{800};
    const int PLAYER_SPRITE_COLUMNS{6};
    const int PLAYER_SPRITE_ROWS{10};
    const float FRAME_TIME{0.1f};

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Prince");

    Texture2D characterSheet = LoadTexture("resources/Player/Player.png");

    int frameWidth = characterSheet.width / PLAYER_SPRITE_COLUMNS;
    int frameHeight = characterSheet.height / PLAYER_SPRITE_ROWS;

    PlayerState playerState{IDLE};
    Vector3 playerPosition = {0.0f, 1.0f, 0.0f};

    int currentFrame{0};
    float frameCounter{0.0f};
    Rectangle frameRectangle = {0.0f, 0.0f, (float)frameWidth,
                                (float)frameHeight};

    Vector3 cameraOffset = {0.0f, 25.0f, 30.0f};
    float moveSpeed = 0.2f;
    float zoomSpeed = 1.0f;
    float minimumZoom = 5.0f;
    float maximumZoom = 30.0f;

    Camera3D camera = {0};
    camera.target = playerPosition;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float yToZRatio = cameraOffset.y / cameraOffset.z;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector3 playerMovement = {0.0f, 0.0f, 0.0f};
        bool isMoving = false;

        if (IsKeyDown(KEY_W)) {
            playerMovement.z -= moveSpeed;
            playerState = WALK_UP;
            isMoving = true;
        }
        if (IsKeyDown(KEY_S)) {
            playerMovement.z += moveSpeed;
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
        playerPosition.z += playerMovement.z;

        float wheelMove = GetMouseWheelMove();
        cameraOffset.z -= wheelMove * zoomSpeed;
        cameraOffset.y -= wheelMove * zoomSpeed;

        if (cameraOffset.z < minimumZoom)
            cameraOffset.z = minimumZoom;
        if (cameraOffset.z > maximumZoom)
            cameraOffset.z = maximumZoom;

        // Adjust the Y-coordinate proportionally to maintain the zoom effect
        cameraOffset.y = cameraOffset.z * yToZRatio;

        camera.position =
            (Vector3){playerPosition.x + cameraOffset.x, cameraOffset.y,
                      playerPosition.z + cameraOffset.z};
        camera.target = playerPosition;

        // Frame animation logic
        frameCounter += GetFrameTime();
        if (frameCounter >= FRAME_TIME) {
            frameCounter = 0.0f;
            currentFrame++;

            // Loop back to the first frame
            if (currentFrame >= PLAYER_SPRITE_COLUMNS)
                currentFrame = 0;
        }

        switch (playerState) {
            case IDLE:
                frameRectangle.y = 0.0f; // 1st row
                break;
            case WALK_DOWN:
                frameRectangle.y = (float)frameHeight * 3; // 4th row
                break;
            case WALK_UP:
                frameRectangle.y = (float)frameHeight * 5; // 6th row
                break;
            case WALK_LEFT:
                frameRectangle.y = (float)frameHeight * 4; // 5th row
                frameRectangle.width = -(float)frameWidth;
                break;
            case WALK_RIGHT:
                frameRectangle.y = (float)frameHeight * 4; // 5th row
                frameRectangle.width = (float)frameWidth;
                break;
        }

        // Update the frame to make it animated
        frameRectangle.x = (float)(currentFrame * frameWidth);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);

        DrawBillboardRec(camera, characterSheet, frameRectangle, playerPosition,
                         (Vector2){2.0f, 2.0f}, WHITE);
        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(characterSheet);
    CloseWindow();
    return 0;
}
