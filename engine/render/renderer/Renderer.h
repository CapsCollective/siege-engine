//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_H
#define SIEGE_ENGINE_RENDERER_H

#include <utils/Logging.h>
#include <window/Window.h>

#include "camera/Camera.h"
#include "lights/PointLight.h"
#include "render/renderer/platform/vulkan/CommandBuffer.h"
#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/DescriptorPool.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

namespace Siege
{

class Renderer
{
public:

    explicit Renderer(Window& window);

    ~Renderer();

    void SetCamera3D(const Camera& camera)
    {
        camera3D = camera;
    }

    void SetCamera2D(const Camera& camera)
    {
        camera2D = camera;
    }

    bool StartFrame();
    void EndFrame();

    void ClearQueues();

    void DrawQuad(const Vec2 position,
                  const Vec2 scale = {1.f, 1.f},
                  const IColour colour = IColour::White,
                  float rotation = 0.f,
                  const uint8_t zIndex = 0,
                  Vulkan::Texture2D* texture = nullptr);
    void DrawQuad(const Vec2 position,
                  Vulkan::TextureAtlas::SubTextureRef texture,
                  const Vec2 scale = {1.f, 1.f},
                  const IColour colour = IColour::White,
                  float rotation = 0.f,
                  const uint8_t zIndex = 0);
    void DrawText2D(const char* text,
                    Vulkan::Font& font,
                    const Vec2 position,
                    const Vec2 scale,
                    float rotation = 0.f,
                    const IColour colour = IColour::White,
                    const uint8_t zIndex = 0);
    void DrawGrid2D(float spacing,
                    const Vec3& lineColouring,
                    float scale,
                    float lineWidth = 1.f,
                    float fadeFactor = 0.5f,
                    float cellMultiple = 2.f);

    static Vulkan::Context& Context()
    {
        return Get()->context;
    }

    static Renderer* Get()
    {
        return instance;
    }

    static uint32_t GetCurrentFrameIndex()
    {
        return currentFrameIndex;
    }

private:

    static Renderer* instance;
    static Vulkan::CommandBuffer commandBuffers;
    static uint32_t currentFrameIndex;

    void RecreateSwapChain();

    void BeginSwapChainRenderPass();
    void EndSwapChainRenderPass();

    void DrawFrame();

    Vulkan::Context context;
    Renderer2D renderer2D;

    Window& window;

    uint32_t currentImageIndex;
    bool isFrameStarted {false};
    Camera camera3D;
    Camera camera2D;
};
} // namespace Siege

#endif
