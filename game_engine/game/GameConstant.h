//
// Created by xinyi wang on 5/5/26.
//

#ifndef GAME_ENGINE_GAMECONSTANT_H
#define GAME_ENGINE_GAMECONSTANT_H

namespace Game
{
    // originally for constants that need to be shared across files without creating file dependencies
    constexpr float GRAVITY = -1.0f;
    constexpr float PLAYER_JUMP_VELOCITY = 0.2f;
    constexpr float PLAYER_MOVEMENT_SPEED = 0.4f;
}
#endif //GAME_ENGINE_GAMECONSTANT_H