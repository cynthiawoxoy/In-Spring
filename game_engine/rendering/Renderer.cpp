//
// Created by greg8 on 1/27/2026.
//

#include <cmath>

#include "Renderer.h"
#include "raylib.h"

namespace Rendering
{
    void Renderer::draw(FrameData& frameData, World& world, uint64_t drawDurationUs,
        std::function<void()> onCustomDraw)
    {
        float windowWidthPx  = static_cast<float>(GetScreenWidth());
        float windowHeightPx = static_cast<float>(GetScreenHeight());
        float minDim = std::min(windowWidthPx, windowHeightPx);
        pxPerGameUnit = minDim / 2;
        originXPx = windowWidthPx / 2;
        originYPx = windowHeightPx / 2;

        BeginDrawing();
        // TODO Parameterize
        ClearBackground(::RAYWHITE);

        // TODO type -> render layer map
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE,
                      [&](Entity entity)
                      {
                          drawEntity(entity, world);
                      });

        if (onCustomDraw) onCustomDraw(); // for tail rendering

        DrawText(TextFormat("FPS %f\nJitter: %lld us\nWork: %lld us\nDraw: %lld us", frameData.fps, frameData.jitterUs,
                            frameData.workDurationUs, drawDurationUs), 0, 0, 36, ::GRAY);

        EndDrawing();
    }

    void Renderer::drawEntity(Entity entity, World& world)
    {
        float objXPx = originXPx + (entity.x() - viewPointX) * pxPerGameUnit;
        float objYPx = originYPx - (entity.y() - viewPointY) * pxPerGameUnit;
        float objWidthPx = entity.width()  * pxPerGameUnit;
        float objHeightPx = entity.height() * pxPerGameUnit;

        // Animation* animation = entity.hasAnimation()
        //                            ? world.animationSystem.getAnimation(entity.animation().animationId)
        //                            : nullptr;
        // Sprite* sprite = entity.hasSprite()
        //                      ? spriteManager.getSprite(entity.spriteId())
        //                      : nullptr;

    //     if (sprite)
    //     {
    //         Rectangle srcRect;
    //         if (animation)
    //             srcRect = sprite->GetSourceRect(animation->getCurrentFrameIndex(entity.animation().progress));
    //         else
    //             srcRect = sprite->GetSourceRect();
    //         DrawTexturePro(sprite->texture2D, srcRect, Rectangle{
    //                            static_cast<float>(objXPx - objWidthPx / 2),
    //                            static_cast<float>(objYPx - objHeightPx / 2),
    //                            static_cast<float>(objWidthPx),
    //                            static_cast<float>(objHeightPx)
    //                        },
    //                        {},
    //                        0,
    //                        entity.hasColor() ? std::bit_cast<::Color>(entity.color()) : WHITE);
    //     }
    //     else
    //     {
    DrawRectangle(static_cast<int>(std::round(objXPx - objWidthPx / 2)),
                  static_cast<int>(std::round(objYPx - objHeightPx / 2)),
                  static_cast<int>(std::round(objWidthPx)),
                  static_cast<int>(std::round(objHeightPx)),
                  entity.hasColor() ? std::bit_cast<::Color>(entity.color()) : BLACK);
    //     }
    }
}
