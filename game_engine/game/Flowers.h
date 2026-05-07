//
// Created by xinyi wang on 5/7/26.
//

#ifndef GAME_ENGINE_FLOWERTRIGGER_H
#define GAME_ENGINE_FLOWERTRIGGER_H

#include <cmath>
#include <vector>
#include "../engine_core/ecs/ECSTypes.h"

namespace Game
{
    constexpr float FLOWER_TRIGGER_RAD = 0.06f;
    constexpr float PEDAL_WIDTH = 0.15f;
    constexpr float PEDAL_HEIGHT = 0.3f;

    constexpr float PEDAL_DISTANCE = 0.02f;
    constexpr float PETAL_ROTATION_TARGET = M_PI / 3;
    constexpr float PETAL_ROTATION_SPEED  = 2.0f;

    struct FlowerTrigger
    {
        float x, y;
        float flowerX, flowerY; // flower position is different from trigger position
        float triggerRadius = FLOWER_TRIGGER_RAD;
        float petalDistance = PEDAL_DISTANCE;
        bool activated = false;
        float rotationAngle = 0;
        std::vector<EngineCore::EntityId> petalIds;
        std::vector<float> petalInitialAngles;
    };
}

#endif //GAME_ENGINE_FLOWERTRIGGER_H
