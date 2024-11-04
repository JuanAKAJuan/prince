#include "../include/Player.hpp"

void Player::updateAnimation(float frameWidth, float frameHeight) {
    frameCounter_ += GetFrameTime();
    if (frameCounter_ >= GameConstants::FRAME_TIME) {
        frameCounter_ = 0.0f;
        currentFrame_ =
            (currentFrame_ + 1) % GameConstants::PLAYER_SPRITE_COLUMNS;
    }
    updateFrameRectangle(frameWidth, frameHeight);
}

void Player::updateFrameRectangle(float frameWidth, float frameHeight) {
    frameRect_.x = static_cast<float>(currentFrame_ * frameWidth);
    frameRect_.width = frameWidth;
    frameRect_.height = frameHeight;

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
