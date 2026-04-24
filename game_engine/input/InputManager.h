//
// Created by greg8 on 1/27/2026.
//

#ifndef INPUT_INPUTMANAGER_H
#define INPUT_INPUTMANAGER_H

namespace Input
{
    struct InputManager
    {
        bool moveLeft = false;
        bool moveRight = false;
        // bool standUp = false;
        // bool getDown = false;

        bool jump = false;

        void update();
    };
}

#endif //INPUT_INPUTMANAGER_H
