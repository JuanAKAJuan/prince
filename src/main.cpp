#include "raylib.h"
#include <algorithm>
#include <array>

namespace GameConstants {
constexpr int SCREEN_WIDTH{1280};
constexpr int SCREEN_HEIGHT{800};
constexpr int PLAYER_SPRITE_COLUMNS{6};
constexpr int PLAYER_SPRITE_ROWS{10};
constexpr float FRAME_TIME{0.1f};
constexpr int MAP_ROWS{10};
constexpr int MAP_COLS{10};
constexpr int TILE_SIZE{32};
constexpr float MOVE_SPEED{TILE_SIZE * 0.1f};
constexpr float ZOOM_SPEED{1.0f};
constexpr float MIN_ZOOM{5.0f};
constexpr float MAX_ZOOM{30.0f};
constexpr float PLAYER_SCALE{2.0f};
} // namespace GameConstants

enum class PlayerState { IDLE, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN };

class Player {
public:
    PlayerState state_{PlayerState::IDLE};
    Vector2 position_;
    int currentFrame_{0};
    float frameCounter_{0.0f};
    Rectangle frameRect_ = {0};

    void updateAnimation(float frameWidth, float frameHeight) {
        frameCounter_ += GetFrameTime();
        if (frameCounter_ >= GameConstants::FRAME_TIME) {
            frameCounter_ = 0.0f;
            currentFrame_ =
                (currentFrame_ + 1) % GameConstants::PLAYER_SPRITE_COLUMNS;
        }
        updateFrameRectangle(frameWidth, frameHeight);
    }

private:
    void updateFrameRectangle(float frameWidth, float frameHeight) {
        frameRect_.x = static_cast<float>(currentFrame_ * frameWidth);
        frameRect_.width = frameWidth;

        switch (state_) {
            case PlayerState::IDLE:
                frameRect_.y = 0.0f;
                break;
            case PlayerState::WALK_DOWN:
                frameRect_.y = frameHeight * 3;
                break;
            case PlayerState::WALK_UP:
                frameRect_.y = frameHeight * 5;
                break;
            case PlayerState::WALK_LEFT:
                frameRect_.y = frameHeight * 4;
                frameRect_.width = -frameRect_.width;
                frameRect_.x += frameWidth;
                break;
            case PlayerState::WALK_RIGHT:
                frameRect_.y = frameHeight * 4;
                break;
        }
    }
};

class Game {
public:
    Game() {
        SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
        InitWindow(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT,
                   "Prince");
        SetTargetFPS(60);
        loadResources();
        initializeGame();
    }

    ~Game() {
        unloadResources();
        CloseWindow();
    }

    void run() {
        while (!WindowShouldClose()) {
            update();
            render();
        }
    }

private:
    Texture2D characterSheet_;
    std::array<Texture2D, 2> tileTextures_;
    Player player_;
    Camera2D camera_{};

    static constexpr std::array<std::array<int, GameConstants::MAP_COLS>,
                                GameConstants::MAP_ROWS>
        tileMap_ = {{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
                     {1, 0, 1, 1, 0, 0, 1, 1, 1, 1},
                     {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
                     {1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                     {1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
                     {1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
                     {0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
                     {1, 1, 0, 0, 0, 0, 0, 0, 1, 1}}};

    void loadResources() {
        characterSheet_ = LoadTexture("resources/Player/Player.png");
        tileTextures_[0] = LoadTexture("resources/Tiles/Grass_Middle.png");
        tileTextures_[1] = LoadTexture("resources/Tiles/Water_Middle.png");
    }

    void unloadResources() {
        UnloadTexture(characterSheet_);
        for (const auto& texture : tileTextures_) {
            UnloadTexture(texture);
        }
    }

    void initializeGame() {
        player_.position_ = {
            GameConstants::MAP_COLS * GameConstants::TILE_SIZE / 2.0f,
            GameConstants::MAP_ROWS * GameConstants::TILE_SIZE / 2.0f};

        camera_.target = player_.position_;
        camera_.offset = {static_cast<float>(GetRenderWidth()) / 2.0f,
                          static_cast<float>(GetRenderHeight()) / 2.0f};
        camera_.zoom = 1.0f;
    }

    void updatePlayer() {
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

    void updateCamera() {
        float wheelMove{GetMouseWheelMove()};
        camera_.zoom =
            std::clamp(camera_.zoom + wheelMove * GameConstants::ZOOM_SPEED,
                       GameConstants::MIN_ZOOM, GameConstants::MAX_ZOOM);

        camera_.target = player_.position_;
        camera_.offset = {static_cast<float>(GetRenderWidth()) / 2.0f,
                          static_cast<float>(GetRenderHeight()) / 2.0f};
    }

    void update() {
        updatePlayer();
        updateCamera();

        const int frameWidth{characterSheet_.width /
                             GameConstants::PLAYER_SPRITE_COLUMNS};
        const int frameHeight{characterSheet_.height /
                              GameConstants::PLAYER_SPRITE_ROWS};
        player_.updateAnimation(frameWidth, frameHeight);
    }

    void renderMap() const {
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

    void renderPlayer() const {
        const float frameWidth{static_cast<float>(characterSheet_.width) /
                               GameConstants::PLAYER_SPRITE_COLUMNS};
        const float frameHeight{static_cast<float>(characterSheet_.height) /
                                GameConstants::PLAYER_SPRITE_ROWS};
        const Rectangle destRect = {player_.position_.x, player_.position_.y,
                                    frameWidth * GameConstants::PLAYER_SCALE,
                                    frameHeight * GameConstants::PLAYER_SCALE};

        const Vector2 origin = {
            frameWidth * GameConstants::PLAYER_SCALE / 2.0f,
            frameHeight * GameConstants::PLAYER_SCALE / 2.0f,
        };

        DrawTexturePro(characterSheet_, player_.frameRect_, destRect, origin,
                       0.0f, WHITE);
    }

    void render() {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode2D(camera_);
        renderMap();
        renderPlayer();
        EndMode2D();

        DrawFPS(10, 10);
        EndDrawing();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
