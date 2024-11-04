#pragma once

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
