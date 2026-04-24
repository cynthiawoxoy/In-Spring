//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "../engine_runtime/EngineRuntime.h"

namespace Game
{
    using namespace EngineRuntime;

    constexpr float GRAVITY = -2.0f;

    struct Game final : IGame
    {
        Runtime runtime;

        EntityId playerId;

        Archetype* borderArchetype;
        Archetype* playerArchetype;
        Archetype* platformArchetype;

        SpriteId playerRunSpriteSheetId;

        AnimationId playerRunAnimationId;

        void onStart(World& world) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        void onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions) override;
        void onUpdatePostCollisionResolution(World& world, std::vector<Collision>& vector) override;
        int run();

        void onUpdatePlayer(World& world);

    };
}

#endif //GAME_ENGINE_GAME_H
