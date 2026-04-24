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
