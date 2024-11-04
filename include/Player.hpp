#pragma once
#include "GameConstants.hpp"
#include "PlayerState.hpp"
#include "raylib.h"

class Player {
public:
    PlayerState state_{PlayerState::IDLE};
    Vector2 position_;
    int currentFrame_{0};
    float frameCounter_{0.0f};
    Rectangle frameRect_ = {0};

    void updateAnimation(float frameWidth, float frameHeight);

private:
    void updateFrameRectangle(float frameWidth, float frameHeight);
};
