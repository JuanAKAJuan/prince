#include "../include/Game.hpp"
#include <algorithm>
#include <raylib.h>

Game::Game() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT,
               "Prince");
    SetWindowMonitor(0);
    SetTargetFPS(60);
    loadResources();
    initializeGame();
}

Game::~Game() {
    unloadResources();
    CloseWindow();
}

void Game::run() {
    while (!WindowShouldClose()) {
        update();
        render();
    }
}

void Game::loadResources() {
    characterSheet_ = LoadTexture("resources/Player/Player.png");
    tileTextures_[0] = LoadTexture("resources/Tiles/Grass_Middle.png");
    tileTextures_[1] = LoadTexture("resources/Tiles/Water_Middle.png");
}

void Game::unloadResources() {
    UnloadTexture(characterSheet_);
    for (const auto& texture : tileTextures_) {
        UnloadTexture(texture);
    }
}

void Game::initializeGame() {
    player_.position_ = {
        GameConstants::MAP_COLS * GameConstants::TILE_SIZE / 2.0f,
        GameConstants::MAP_ROWS * GameConstants::TILE_SIZE / 2.0f};

    camera_.target = player_.position_;
    camera_.offset = {static_cast<float>(GetRenderWidth()) / 2.0f,
                      static_cast<float>(GetRenderHeight()) / 2.0f};
    camera_.zoom = 1.0f;
}

void Game::updatePlayer() {
    Vector2 movement = {0.0f, 0.0f};
    bool isMoving{false};

    if (IsKeyDown(KEY_W)) {
        movement.y -= GameConstants::MOVE_SPEED;
        player_.state_ = PlayerState::WALK_UP;
        isMoving = true;
    }
    if (IsKeyDown(KEY_S)) {
        movement.y += GameConstants::MOVE_SPEED;
        player_.state_ = PlayerState::WALK_DOWN;
        isMoving = true;
    }
    if (IsKeyDown(KEY_A)) {
        movement.x -= GameConstants::MOVE_SPEED;
        player_.state_ = PlayerState::WALK_LEFT;
        isMoving = true;
    }
    if (IsKeyDown(KEY_D)) {
        movement.x += GameConstants::MOVE_SPEED;
        player_.state_ = PlayerState::WALK_RIGHT;
        isMoving = true;
    }

    if (!isMoving) {
        player_.state_ = PlayerState::IDLE;
    }

    player_.position_.x += movement.x;
    player_.position_.y += movement.y;
}

void Game::updateCamera() {
    float wheelMove{GetMouseWheelMove()};
    camera_.zoom =
        std::clamp(camera_.zoom + wheelMove * GameConstants::ZOOM_SPEED,
                   GameConstants::MIN_ZOOM, GameConstants::MAX_ZOOM);

    camera_.target = player_.position_;
    camera_.offset = {static_cast<float>(GetRenderWidth()) / 2.0f,
                      static_cast<float>(GetRenderHeight()) / 2.0f};
}

void Game::update() {
    updatePlayer();
    updateCamera();

    const int frameWidth{characterSheet_.width /
                         GameConstants::PLAYER_SPRITE_COLUMNS};
    const int frameHeight{characterSheet_.height /
                          GameConstants::PLAYER_SPRITE_ROWS};
    player_.updateAnimation(frameWidth, frameHeight);
}

void Game::renderMap() const {
    for (int y = 0; y < GameConstants::MAP_ROWS; y++) {
        for (int x = 0; x < GameConstants::MAP_COLS; x++) {
            int tileType{tileMap_[y][x]};
            Vector2 position = {
                static_cast<float>(x * GameConstants::TILE_SIZE),
                static_cast<float>(y * GameConstants::TILE_SIZE)};
            const float scale{static_cast<float>(GameConstants::TILE_SIZE) /
                              tileTextures_[tileType].width};

            DrawTextureEx(tileTextures_[tileType], position, 0.0f, scale,
                          WHITE);
        }
    }
}

void Game::renderPlayer() const {
    const float frameWidth{static_cast<float>(characterSheet_.width) /
                           GameConstants::PLAYER_SPRITE_COLUMNS};
    const float frameHeight{static_cast<float>(characterSheet_.height) /
                            GameConstants::PLAYER_SPRITE_ROWS};
    const Rectangle destRect = {player_.position_.x, player_.position_.y,
                                frameWidth * GameConstants::PLAYER_SCALE,
                                frameHeight * GameConstants::PLAYER_SCALE};

    const Vector2 origin = {frameWidth * GameConstants::PLAYER_SCALE / 2.0f,
                            frameHeight * GameConstants::PLAYER_SCALE / 2.0f};

    DrawRectangleLines(static_cast<int>(destRect.x - origin.x),
                       static_cast<int>(destRect.y - origin.y),
                       static_cast<int>(destRect.width),
                       static_cast<int>(destRect.height), RED);

    DrawTexturePro(characterSheet_, player_.frameRect_, destRect, origin, 0.0f,
                   WHITE);
}

void Game::render() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    BeginMode2D(camera_);
    renderMap();
    renderPlayer();
    EndMode2D();

    DrawFPS(10, 10);
    EndDrawing();
}
