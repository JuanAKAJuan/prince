#pragma once
#include "GameConstants.hpp"
#include "Player.hpp"
#include "raylib.h"
#include <array>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void loadResources();
    void unloadResources();
    void initializeGame();
    void updatePlayer();
    void updateCamera();
    void update();
    void renderMap() const;
    void renderPlayer() const;
    void render();

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
};
