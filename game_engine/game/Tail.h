//
// Created by xinyi wang on 5/5/26.
//

#ifndef GAME_ENGINE_TAIL_H
#define GAME_ENGINE_TAIL_H



namespace Game
{
    struct TailPoint
    {
        float x, y;
        float prevX, prevY;
    };

    constexpr int TAIL_POINT_COUNT = 3;
    constexpr float TAIL_SEGMENT_LENGTH = 0.06f; // expected length between two consecutive points of tail
    constexpr float LATCH_TAIL_GRAVITY = -4.0f;  // modify "heaviness" of tail
    constexpr float UNLATCH_TAIL_GRAVITY = -2.3f;
    constexpr float TAIL_LENGTH = TAIL_POINT_COUNT * TAIL_SEGMENT_LENGTH; // for set up flower trigger position underneath platform

    struct Tail
    {
        TailPoint tailPoints[TAIL_POINT_COUNT];

        void init(float anchorX, float anchorY);
        void update(float anchorX, float anchorY, float dt, bool isPlayerGrounded);
    };
}


#endif //GAME_ENGINE_TAIL_H