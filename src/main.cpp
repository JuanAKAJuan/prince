#include "raylib.h"

int main() {
    const int screenWidth{1280};
    const int screenHeight{800};

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(screenWidth, screenHeight, "Prince");

    Mesh terrainMesh = GenMeshPlane(50.0f, 50.0f, 10, 10);
    Model terrainModel = LoadModelFromMesh(terrainMesh);
    Texture2D terrainTexture = LoadTexture("resources/ground_terrain.png");
    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTexture;

    Vector3 terrainPosition = {0.0f, 0.0f, 0.0f};

    Vector3 cubePosition = {0.0f, 1.0f, 0.0f};
    Vector3 cameraOffset = {0.0f, 25.0f, 30.0f};
    float moveSpeed = 0.2f;
    float zoomSpeed = 1.0f;
    float minimumZoom = 5.0f;
    float maximumZoom = 30.0f;

    Camera3D camera = {0};
    camera.target = cubePosition;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float yToZRatio = cameraOffset.y / cameraOffset.z;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W))
            cubePosition.z -= moveSpeed;
        if (IsKeyDown(KEY_S))
            cubePosition.z += moveSpeed;
        if (IsKeyDown(KEY_A))
            cubePosition.x -= moveSpeed;
        if (IsKeyDown(KEY_D))
            cubePosition.x += moveSpeed;

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
            (Vector3){cubePosition.x + cameraOffset.x, cameraOffset.y,
                      cubePosition.z + cameraOffset.z};
        camera.target = cubePosition;

        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);
        DrawModel(terrainModel, terrainPosition, 1.0f, WHITE);
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        EndMode3D();

        DrawText("Cool 3D cube, jk :P", 10, 40, 20, RAYWHITE);
        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(terrainTexture);
    UnloadModel(terrainModel);
    CloseWindow();
    return 0;
}
