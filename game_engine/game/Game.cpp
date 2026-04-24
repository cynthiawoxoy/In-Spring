//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"

#include <cmath>
#include "../engine_core/util/Random.h"

namespace Game
{
    int Game::run()
    {
        return runtime.run(*this, 1600, 1200, "GEP");
    }

    void Game::onStart(World& world)
    {
        borderArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);
        playerArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_SPRITE);
        Archetype* platformArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_SPRITE);

        /* TODO: DELETE ME
        *enum Components : ComponentMask
        {
            COMP_POSITION = 1 << 0,
            COMP_VELOCITY = 1 << 1,
            COMP_ACCELERATION = 1 << 2,
            COMP_SIZE = 1 << 3,
            COMP_COLOR = 1 << 4,
            COMP_STATE = 1 << 5,
            COMP_COLLIDER = 1 << 6,
            COMP_ANIMATION = 1 << 7,
            COMP_SPRITE = 1 << 8,
        };
         */

        playerId = world.createEntity(playerArchetype,
                                      0, 0,
                                      0, 0,
                                      0.2, 0.2,
                                      white,
                                      Entity::STATE_DEFAULT,
                                      ColliderShape::RECT, ColliderLayers::LAYER_PLAYER,
                                      {},
                                      playerRunSpriteSheetId
                                      // INVALID_SPRITE_ID // TODO: sprite
                                      );

        // Double-unit "border" squares
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           2, 2,
                           yellow,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID);
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           1.95, 1.95,
                           rayWhite,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT, LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID);

        for (int i = 1; i < 10; i++)
            world.createEntity(platformArchetype,
                               0, 0, // TODO: change platform positions and locations
                               0, 0,
                               0.35, 0.08,
                               white,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT, LAYER_PLATFORMS,
                               {},
                               ColliderLayers::LAYER_PLATFORMS);
    }

    void Game::onUpdateBegin(World& world)
    {
        onUpdatePlayer(world);

    }

    void Game::onUpdatePostKinematics(World& world)
    {
    }

    void Game::onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions)
    {
    }

    void Game::onUpdatePostCollisionResolution(World& world, std::vector<Collision>& collisions)
    {

    }

    void Game::onUpdatePlayer(World& world)
    {
        // Input directly affects player velocity.
        Entity player = world.findEntity(playerId).value();
        player.vx() = 0;
        if (runtime.inputManager.moveLeft)
            player.vx() = -1;
        if (runtime.inputManager.moveRight)
            player.vx() = 1;

        player.vy() = 0;
        // TODO: should I check if player on a platform here?


        /*
        if (runtime.inputManager.standUp)
            // TODO: switch to standup position and show a dialoge
        if (runtime.inputManager.getDown)
            // TODO: get down
        */


        if (runtime.inputManager.jump)
        {

        }
    }
}
