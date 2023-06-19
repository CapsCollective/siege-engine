//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_H
#define SIEGE_ENGINE_RENDERER_H

#include <utils/Logging.h>
#include <window/Window.h>

#include "lights/PointLight.h"
#include "model/Model.h"
#include "render/renderer/platform/vulkan/CommandBuffer.h"
#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/DescriptorPool.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "renderer/Common.h"

namespace Siege
{

class Renderer
{
public:

    explicit Renderer(Window& window);

    ~Renderer();

    float GetAspectRatio() const
    {
        return static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
    }

    void SetProjection(const Mat4& projectionMat, const Mat4& viewMat)
    {
        projection = {projectionMat, viewMat};
    }

    void SetUiProjection(const Mat4& projectionMat, const Mat4& viewMat)
    {
        uiCamera = {projectionMat, viewMat};
    }

    bool StartFrame();
    void EndFrame();

    void ClearDeviceQueue();

    void DrawQuad(const Vec2 position,
                  const Vec2 scale = {1.f, 1.f},
                  const IColour colour = IColour::White,
                  float rotation = 0.f,
                  const uint8_t zIndex = 0,
                  Vulkan::Texture2D* texture = nullptr);
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
    CameraData projection;
    CameraData uiCamera;
};
} // namespace Siege

#endif
