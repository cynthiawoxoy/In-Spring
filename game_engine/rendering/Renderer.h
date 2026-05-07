//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_RENDERER_H
#define GAME_ENGINE_RENDERER_H

#include <functional>
#include "engine_core/EngineCore.h"

#include "SpriteManager.h"

namespace Rendering
{
    using namespace EngineCore;

    struct Renderer final
    {
        SpriteManager spriteManager;

        float viewPointX = 0, viewPointY = 0;
        float pxPerGameUnit = 0;
        float originXPx = 0, originYPx = 0;

        void draw(FrameData& frameData, World& world, uint64_t drawDurationUs,
                std::function<void()> onCustomDraw = nullptr); // to allow rendering for tail

    private:
        void drawEntity(Entity entity, World& world);
    };
}

#endif //GAME_ENGINE_RENDERER_H
