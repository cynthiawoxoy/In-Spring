//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"
#include "GameConstant.h"

#include <cmath>
#include "../engine_core/util/Random.h"

// TODO: see if random.h is useful. DELETE ME

namespace Game
{
    int Game::run()
    {
        return runtime.run(*this, 1600, 1200, "GEP");
    }

    void Game::onStart(World& world)
    {
        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_PLATFORMS, true);

        borderArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);
        playerArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_VELOCITY
            | Archetype::COMP_ACCELERATION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLLIDER
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_SPRITE);
        platformArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR
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
                                      START_POSITION_X + PLAYER_WIDTH / 2,
                                      START_POSITION_Y + PLATFORM_HEIGHT + PLAYER_HEIGHT,
                                      0, 0,
                                      0, 0,
                                      PLAYER_WIDTH, PLAYER_HEIGHT,
                                      white,
                                      Entity::STATE_DEFAULT,
                                      ColliderShape::RECT,
                                      ColliderLayers::LAYER_PLAYER,
                                      {},
                                      INVALID_SPRITE_ID // TODO: sprite
                                      );

        Entity player = world.findEntity(playerId).value();
        tail.init(player.x(), player.bottom());

        // Double-unit "border" squares
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           0, 0,
                           2, 2,
                           yellow,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT,
                           LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID);
        world.createEntity(borderArchetype,
                           0, 0,
                           0, 0,
                           0, 0,
                           1.95, 1.95,
                           rayWhite,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT,
                           LAYER_NONE,
                           {},
                           INVALID_SPRITE_ID);

        createPlatforms(world);
    }

    void Game::onUpdateBegin(World& world)
    {
        onUpdatePlayer(world);

    }

    void Game::onUpdatePostKinematics(World& world)
    {
        Entity player = world.findEntity(playerId).value();
        tail.update(player.x(), player.bottom(), world.frameDt, isPlayerGrounded());
    }

    void Game::onUpdatePostCollisionDetection(World& world, std::vector<Collision>& collisions)
    {
    }

    // Assume the game will only have one player, here if isPlayer, skip entire players' layer
    // Assume if player is latched, the world map ensures no further possible collision between player and platforms
    bool Game::skipLayerLevelCollisionDetection(Entity& entityA, Entity& entityB)
    {
        // If player is grounded, check if passed in Entities are the player and the latchedPlatform.
        // Skip collision check between player and platforms
        if (isPlayerGrounded())
        {
            if ((entityA.colliderLayerId() == LAYER_PLAYER && entityB.id() == latchedToPlatform) ||
                (entityB.colliderLayerId() == LAYER_PLAYER && entityA.id() == latchedToPlatform)) return true;
        }
        return false;
    }

    bool Game::skipEntityLevelCollisionDetection(Entity& entityA, Entity& entityB)
    {
        if ((entityA.colliderLayerId() == LAYER_PLAYER && entityB.colliderLayerId() == LAYER_PLATFORMS) ||
            (entityB.colliderLayerId() == LAYER_PLAYER && entityA.colliderLayerId() == LAYER_PLATFORMS))
        {
            Entity& player = entityA.colliderLayerId() == LAYER_PLAYER ? entityA : entityB;
            Entity& platform = entityA.colliderLayerId() == LAYER_PLAYER ? entityB : entityA;
            // skip the case player jump from bottom to top of platform
            bool isLanding = playerPrevBottom >= platform.top() &&
                                player.bottom() <= platform.top() &&
                                player.vy() <= 0;
            return !isLanding;
        }

        return false;
    }

    void Game::onUpdatePostCollisionResolution(World& world, std::vector<Collision>& collisions)
    {
        for (auto& collision : collisions)
        {
            auto optA = world.findEntity(collision.a);
            auto optB = world.findEntity(collision.b);
            if (!optA || !optB) continue;

            Entity& entityA = optA.value();
            Entity& entityB = optB.value();


            // player falls from air to a new platform
            if ((entityA.colliderLayerId() == LAYER_PLAYER && entityB.colliderLayerId() == LAYER_PLATFORMS) ||
            (entityB.colliderLayerId() == LAYER_PLAYER && entityA.colliderLayerId() == LAYER_PLATFORMS))
            {
                Entity& player = entityA.colliderLayerId() == LAYER_PLAYER ? entityA : entityB;
                Entity& platform = entityA.colliderLayerId() == LAYER_PLAYER ? entityB : entityA;

                latchedToPlatform = platform.id();
                player.vy() = 0;
            }
        }


        // update viewPoint for Renderer
        auto player = world.findEntity(playerId).value();
        runtime.renderer.viewPointX = player.x();
        runtime.renderer.viewPointY = player.y();
    }

    void Game::onDraw()
    {
        auto& r = runtime.renderer;

      for (int i = 0; i < TAIL_POINT_COUNT - 1; i++)
      {
          DrawLine(
              static_cast<int>(r.originXPx + (tail.tailPoints[i].x   - r.viewPointX) * r.pxPerGameUnit),
              static_cast<int>(r.originYPx - (tail.tailPoints[i].y   - r.viewPointY) * r.pxPerGameUnit),
              static_cast<int>(r.originXPx + (tail.tailPoints[i+1].x - r.viewPointX) * r.pxPerGameUnit),
              static_cast<int>(r.originYPx - (tail.tailPoints[i+1].y - r.viewPointY) * r.pxPerGameUnit),
              BEIGE
          );
      }

      for (int i = 0; i < TAIL_POINT_COUNT; i++)
      {
          DrawCircle(
              static_cast<int>(r.originXPx + (tail.tailPoints[i].x - r.viewPointX) * r.pxPerGameUnit),
              static_cast<int>(r.originYPx - (tail.tailPoints[i].y - r.viewPointY) * r.pxPerGameUnit),
              5.0f,
              BEIGE
          );
      }
    }

    void Game::onUpdatePlayer(World& world)
    {
        // Input directly affects player velocity.
        Entity player = world.findEntity(playerId).value();

        playerPrevBottom = player.bottom();

        if (isPlayerGrounded())
        {
            // check is player unlatched
            Entity platform = world.findEntity(latchedToPlatform).value();
            if (player.right() < platform.left() || player.left() > platform.right())
                latchedToPlatform = INVALID_ENTITY_ID;
        }

        if (runtime.inputManager.jump && isPlayerGrounded())
        {
            player.vy() = PLAYER_JUMP_VELOCITY;
            latchedToPlatform = INVALID_ENTITY_ID;
        }

        // regular onUpdate velocity and acceleration
        if (isPlayerGrounded())
        {   // TODO: tails or player might need positive acceleration in the future, if bug, check here
            player.vy() = 0;
            player.ay() = 0;
        } else { player.ay() = GRAVITY; }

        player.vx() = 0;
        if (runtime.inputManager.moveLeft)
            player.vx() = -PLAYER_MOVEMENT_SPEED;
        else if (runtime.inputManager.moveRight)
            player.vx() = PLAYER_MOVEMENT_SPEED;




        /*
        if (runtime.inputManager.standUp)
            // TODO: switch to standup position and show a dialoge
        if (runtime.inputManager.getDown)
            // TODO: get down
        */

    }

    void Game::createPlatforms(World& world)
    {
        // check writeup for platform planning and its grid numbers
        GameColor platformColor = grassGreen;
        // Grid 1, 2
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 2,
                               START_POSITION_Y,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH * 2, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);

        // Grid 3
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 7,
                               START_POSITION_Y + PLATFORM_GRID_LENGTH / 2,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);

        // Grid 4
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 9,
                               START_POSITION_Y + PLATFORM_GRID_LENGTH * -4,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);

        // Grid 5
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 11.5,
                               START_POSITION_Y + PLATFORM_GRID_LENGTH * -4.5,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);

        // Grid 6
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 14,
                               START_POSITION_Y + PLATFORM_GRID_LENGTH * -5,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);

        // Grid 7
        world.createEntity(platformArchetype,
                               START_POSITION_X + PLATFORM_GRID_LENGTH * 17.5,
                               START_POSITION_Y + PLATFORM_GRID_LENGTH * -4.1,
                               0, 0,
                               0, 0,
                               PLATFORM_WIDTH, PLATFORM_HEIGHT,
                               platformColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::RECT,
                               LAYER_PLATFORMS,
                               {},
                               INVALID_SPRITE_ID);
    }
}
