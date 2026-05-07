//
// Created by xinyi wang on 5/5/26.
//

#ifndef GAME_ENGINE_GAMECONSTANT_H
#define GAME_ENGINE_GAMECONSTANT_H

namespace Game
{
    constexpr float GRAVITY = -1.0f;

    constexpr float PLAYER_WIDTH = 0.2f;
    constexpr float PLAYER_HEIGHT = 0.14f;
    constexpr float PLAYER_JUMP_VELOCITY = 0.5f;

    constexpr float PLAYER_MOVEMENT_SPEED = 0.6f;
    constexpr float PLATFORM_WIDTH = 0.25f;
    constexpr float PLATFORM_HEIGHT = 0.03f;
    constexpr float PLATFORM_GRID_LENGTH = PLATFORM_WIDTH / 2;

    constexpr float START_POSITION_X = -1.0f;
    constexpr float START_POSITION_Y = 0.0f;
}
#endif //GAME_ENGINE_GAMECONSTANT_H