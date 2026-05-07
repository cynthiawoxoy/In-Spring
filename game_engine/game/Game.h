//
// Created by greg8 on 1/29/2026.
//

#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include "../engine_runtime/EngineRuntime.h"
#include "GameConstant.h"
#include "Tail.h"

namespace Game
{
    using namespace EngineRuntime;


    struct Game final : IGame
    {
        Runtime runtime;

        EntityId playerId;
        float playerPrevBottom;
        EntityId latchedToPlatform = INVALID_ENTITY_ID;

        Archetype* borderArchetype;
        Archetype* playerArchetype;
        Archetype* platformArchetype;

        SpriteId playerRunSpriteSheetId;

        AnimationId playerRunAnimationId;

        Tail tail;

        void onStart(World& world) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        // if collision detect between two layers should be skipped, function return true
        bool skipLayerLevelCollisionDetection(Entity& entityA, Entity& entityB) override;
        bool skipEntityLevelCollisionDetection(Entity& entityA, Entity& entityB) override;
        void onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions) override;
        void onUpdatePostCollisionResolution(World& world, std::vector<Collision>& vector) override;
        void onDraw() override;
        int run();

        // simple location, velocity updates
        void onUpdatePlayer(World& world);

        const bool isPlayerGrounded() const { return latchedToPlatform != INVALID_ENTITY_ID; }

        enum Layers : ColliderLayerId
        {
            LAYER_NONE,
            LAYER_PLAYER,
            LAYER_PLATFORMS
        };
    };
}

#endif //GAME_ENGINE_GAME_H
