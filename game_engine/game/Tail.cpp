//
// Created by xinyi wang on 5/5/26.
//

#include "Tail.h"
#include "GameConstant.h"

namespace Game
{
    void Tail::init(float anchorX, float anchorY)
    {
        for (int i = 0; i < TAIL_POINT_COUNT; i++)
        {
            float y = anchorY - TAIL_SEGMENT_LENGTH * i;
            tailPoints[i] = { anchorX, y, anchorX, y };
        }
    }

    void Tail::update(float anchorX, float anchorY, float dt)
    {
        // Pin point 0 to anchor
        tailPoints[0].prevX = tailPoints[0].x;
        tailPoints[0].prevY = tailPoints[0].y;
        tailPoints[0].x = anchorX;
        tailPoints[0].y = anchorY;

        // Verlet integration for free points
        /* newPosition = currPosition + velocity * dt + acceleration * dt ^ 2
         * same as: newPosition = currPosition + momentum  + gravity
         *      velocity * dt = currPosition - prevPosition
         */
        for (int i = 1; i < TAIL_POINT_COUNT; i++)
        {
            float newX = 2 * tailPoints[i].x - tailPoints[i].prevX;
            float newY = 2 * tailPoints[i].y - tailPoints[i].prevY + TAIL_GRAVITY * dt * dt;
            tailPoints[i].prevX = tailPoints[i].x;
            tailPoints[i].prevY = tailPoints[i].y;
            tailPoints[i].x = newX;
            tailPoints[i].y = newY;
        }

        // Constraint relaxation
        // fix error in distance between points due to gravity update, keep consecutive tail points exactly TAIL_SEGMENT_LENGTH apart
        constexpr int CONSTRAINT_ITERATIONS = 5;
        float restLen_sqrd = TAIL_SEGMENT_LENGTH * TAIL_SEGMENT_LENGTH;
        for (int iter = 0; iter < CONSTRAINT_ITERATIONS; iter++)
        {
            for (int i = 1; i < TAIL_POINT_COUNT; i++) // skip tail root
            {
                float dx = tailPoints[i].x - tailPoints[i-1].x;
                float dy = tailPoints[i].y - tailPoints[i-1].y;
                float dist_sqrd = dx*dx + dy*dy;

                if (dist_sqrd < 1e-4f) continue;

                // float correction = (dist - TAIL_SEGMENT_LENGTH) / dist;
                // optimized to avoid sqrt:
                float correction = (dist_sqrd - restLen_sqrd) / (dist_sqrd + restLen_sqrd);

                if (i == 1)
                {
                    // point 0 is pinned, move only point 1
                    tailPoints[i].x -= dx * correction;
                    tailPoints[i].y -= dy * correction;
                }
                else
                {
                    // both free, split equally
                    tailPoints[i-1].x += dx * correction * 0.5f;
                    tailPoints[i-1].y += dy * correction * 0.5f;
                    tailPoints[i].x -= dx * correction * 0.5f;
                    tailPoints[i].y -= dy * correction * 0.5f;
                }
            }
        }
    }
}
