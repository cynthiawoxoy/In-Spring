//
// Created by greg8 on 1/29/2026.
//

#include "Game.h"
#include "GameConstant.h"

#include <cmath>


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
            | Archetype::COMP_COLOR
            | Archetype::COMP_SPRITE);
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

        flowerTriggerArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_COLOR);

        petalArchetype = world.createArchetype(
            Archetype::COMP_POSITION
            | Archetype::COMP_SIZE
            | Archetype::COMP_ANIMATION
            | Archetype::COMP_SPRITE);

        playerSpriteId = runtime.renderer.spriteManager.createSprite("assets/player.png", 1, 1);
        petalSpriteId = runtime.renderer.spriteManager.createSprite("assets/petal.png", 1, 1);
        backgroundSpriteId = runtime.renderer.spriteManager.createSprite("assets/background.jpg", 1, 1);

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
                                      playerSpriteId
                                      );

        Entity player = world.findEntity(playerId).value();
        tail.init(player.x(), player.bottom());

        world.createEntity(borderArchetype,
                           0.1, 0,
                           0, 0,
                           0, 0,
                           2.45, 2.45,
                           rayWhite,
                           Entity::STATE_DEFAULT,
                           ColliderShape::RECT,
                           LAYER_NONE,
                           {},
                           backgroundSpriteId);

        createPlatforms(world);
        createFlowerTriggers(world);
    }

    void Game::onUpdateBegin(World& world)
    {
        onUpdatePlayer(world);

    }

    void Game::onUpdatePostKinematics(World& world)
    {
        Entity player = world.findEntity(playerId).value();
        tail.update(player.x() - PLAYER_WIDTH / 3, player.bottom() + PLAYER_HEIGHT / 2, world.frameDt, isPlayerGrounded());

        TailPoint& tailTip = tail.tailPoints[TAIL_POINT_COUNT - 1];
        // a fake tailTip-trigger "collision check", because tail physics does not fit into ECS system and hard to fit in existing collision detection system
        for (auto& trigger : flowerTriggers)
        {
            if (trigger.activated) continue;
            float dx = tailTip.x - trigger.x;
            float dy = tailTip.y - trigger.y;
            if (dx*dx + dy*dy < trigger.triggerRadius * trigger.triggerRadius) trigger.activated = true;
        }

        // petal rotation animation
        for (auto& trigger : flowerTriggers)
        {
            if (!trigger.activated) continue;

            trigger.rotationAngle += PETAL_ROTATION_SPEED * world.frameDt;


            for (int i = 0; i < trigger.petalIds.size(); i++)
            {
                auto opt = world.findEntity(trigger.petalIds[i]);
                if (!opt) continue;

                Entity& petal = opt.value();
                float angle = trigger.petalInitialAngles[i] + trigger.rotationAngle;
                petal.x() = trigger.flowerX + std::cos(angle) * trigger.petalDistance;
                petal.y() = trigger.flowerY + std::sin(angle) * trigger.petalDistance;
            }

        }
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
            DrawLineEx(Vector2{ r.originXPx + (tail.tailPoints[i].x   - r.viewPointX) * r.pxPerGameUnit,
                 r.originYPx - (tail.tailPoints[i].y   - r.viewPointY) * r.pxPerGameUnit },
                    Vector2{ r.originXPx + (tail.tailPoints[i+1].x - r.viewPointX) * r.pxPerGameUnit,
                    r.originYPx - (tail.tailPoints[i+1].y - r.viewPointY) * r.pxPerGameUnit },
                    TAIL_JOINT_RADIUS * 2,
                    BEIGE);
        }

        for (int i = 0; i < TAIL_POINT_COUNT; i++)
        {
            DrawCircle(
                static_cast<int>(r.originXPx + (tail.tailPoints[i].x - r.viewPointX) * r.pxPerGameUnit),
                static_cast<int>(r.originYPx - (tail.tailPoints[i].y - r.viewPointY) * r.pxPerGameUnit),
                TAIL_JOINT_RADIUS,
                BEIGE);
        }

        // flower animation
        for (auto& trigger : flowerTriggers)
        {
            float screenX = r.originXPx + (trigger.x - r.viewPointX) * r.pxPerGameUnit;
            float screenY = r.originYPx - (trigger.y - r.viewPointY) * r.pxPerGameUnit;
            float screenRadius = trigger.triggerRadius * r.pxPerGameUnit;

            DrawCircleLines(
                static_cast<int>(screenX),
                static_cast<int>(screenY),
                screenRadius,
                trigger.activated ? GREEN : RED);
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
        {
            player.vy() = 0;
            player.ay() = 0;
        } else { player.ay() = GRAVITY; }

        player.vx() = 0;
        if (runtime.inputManager.moveLeft)
            player.vx() = -PLAYER_MOVEMENT_SPEED;
        else if (runtime.inputManager.moveRight)
            player.vx() = PLAYER_MOVEMENT_SPEED;
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

    void Game::createFlowerTriggers(World& world)
    {
        GameColor triggerColor = warmBrown;

        // Trigger 1
        world.createEntity(flowerTriggerArchetype,
                                START_POSITION_X + PLATFORM_GRID_LENGTH * 7.3,
                                START_POSITION_Y + PLATFORM_GRID_LENGTH / 2 - TAIL_LENGTH * 0.6,
                               0, 0,
                               0, 0,
                               FLOWER_TRIGGER_RAD, FLOWER_TRIGGER_RAD,
                               triggerColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::CIRCLE,
                               LAYER_NONE,
                               {},
                               INVALID_SPRITE_ID);
        FlowerTrigger trigger1;
        trigger1.x = START_POSITION_X + PLATFORM_GRID_LENGTH * 7.3;
        trigger1.y = START_POSITION_Y + PLATFORM_GRID_LENGTH / 2 - TAIL_LENGTH * 0.6;
        trigger1.flowerX = START_POSITION_X + PLATFORM_GRID_LENGTH * 9.0f;
        trigger1.flowerY = START_POSITION_Y + PLATFORM_GRID_LENGTH * 2.0f;

        int petalCount = 5;
        for (int i = 0; i < petalCount; i++)
        {
            float angle = (2.0f * M_PI / petalCount) * i;
            float px = trigger1.flowerX + std::cos(angle) * trigger1.petalDistance;
            float py = trigger1.flowerY + std::sin(angle) * trigger1.petalDistance;
            EntityId id = world.createEntity(petalArchetype,
                                        px, py,
                                        0, 0, 0, 0,
                                        PEDAL_WIDTH, PEDAL_HEIGHT,
                                        white,
                                        Entity::STATE_DEFAULT,
                                        ColliderShape::RECT,
                                        LAYER_NONE,
                                        {},
                                        petalSpriteId);
            trigger1.petalIds.push_back(id);
            trigger1.petalInitialAngles.push_back(angle);
        }
        flowerTriggers.push_back(trigger1);

        // Trigger 2
        world.createEntity(flowerTriggerArchetype,
                                START_POSITION_X + PLATFORM_GRID_LENGTH * 13,
                                START_POSITION_Y + PLATFORM_GRID_LENGTH * -4.5,
                               0, 0,
                               0, 0,
                               FLOWER_TRIGGER_RAD, FLOWER_TRIGGER_RAD,
                               triggerColor,
                               Entity::STATE_DEFAULT,
                               ColliderShape::CIRCLE,
                               LAYER_NONE,
                               {},
                               INVALID_SPRITE_ID);
        FlowerTrigger trigger2;
        trigger2.x = START_POSITION_X + PLATFORM_GRID_LENGTH * 13;
        trigger2.y = START_POSITION_Y + PLATFORM_GRID_LENGTH * -4.5;
        trigger2.flowerX = START_POSITION_X + PLATFORM_GRID_LENGTH * 11;
        trigger2.flowerY = START_POSITION_Y + PLATFORM_GRID_LENGTH * -1.5f;
        for (int i = 0; i < petalCount; i++)
        {
            float angle = (2.0f * M_PI / petalCount) * i;
            float px = trigger2.flowerX + std::cos(angle) * trigger2.petalDistance;
            float py = trigger2.flowerY + std::sin(angle) * trigger2.petalDistance;
            EntityId id = world.createEntity(petalArchetype,
                                        px, py,
                                        0, 0, 0, 0,
                                        PEDAL_WIDTH, PEDAL_HEIGHT,
                                        white,
                                        Entity::STATE_DEFAULT,
                                        ColliderShape::RECT,
                                        LAYER_NONE,
                                        {},
                                        petalSpriteId);
            trigger2.petalIds.push_back(id);
            trigger2.petalInitialAngles.push_back(angle);
        }
        flowerTriggers.push_back(trigger2);
    }
}
