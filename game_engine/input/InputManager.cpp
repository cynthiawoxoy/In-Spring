//
// Created by greg8 on 1/27/2026.
//

#include "InputManager.h"

#include "raylib.h"

namespace Input
{
    void InputManager::update()
    {
        this->moveLeft = IsKeyDown(KEY_LEFT);
        this->moveRight = IsKeyDown(KEY_RIGHT);
        // this->getDown = IsKeyDown(KEY_SPACE);
        // this->standUp = IsKeyDown(KEY_SPACE);

        this->jump = IsKeyPressed(KEY_UP);
    }
}
