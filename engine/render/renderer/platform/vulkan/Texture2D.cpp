//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Texture2D.h"

#include "Constants.h"
#include "Context.h"
#include "render/renderer/buffer/Buffer.h"
#include "utils/Descriptor.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <utils/Defer.h>
#include <utils/Logging.h>

namespace Siege::Vulkan
{
Texture2D::Texture2D(const char* name, Usage texUsage)
{
    LoadTexture(Constants::DEFAULT_TEXTURE_2D, Constants::DEFAULT_TEXTURE_SIZE, 16, 16, texUsage);

    VkSamplerCreateInfo samplerInfo = Utils::Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR);

    info = {image.GetInfo()};

    vkCreateSampler(Context::GetVkLogicalDevice(), &samplerInfo, nullptr, &info.sampler);

    // TODO(Aryeh): Make this configurable
    id = INTERN_STR(name);

    info.usage = texUsage;
}

Texture2D::Texture2D(const char* name, const char* filePath)
{
    LoadFromFile(filePath);

    VkSamplerCreateInfo samplerInfo = Utils::Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR);

    info = {image.GetInfo()};

    vkCreateSampler(Context::GetVkLogicalDevice(), &samplerInfo, nullptr, &info.sampler);

    id = INTERN_STR(name);
}

Texture2D::Texture2D(const char* name, const uint8_t* pixels, size_t size, uint32_t width, uint32_t height, Usage texUsage)
{
    LoadTexture(pixels, size, width, height, texUsage);

    VkSamplerCreateInfo samplerInfo = Utils::Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR);

    info = {image.GetInfo()};

    vkCreateSampler(Context::GetVkLogicalDevice(), &samplerInfo, nullptr, &info.sampler);

    info.usage = texUsage;

    id = INTERN_STR(name);
}

Texture2D::Texture2D(Texture2D&& other)
{
    Swap(other);
}

Texture2D::~Texture2D()
{
    Free();
}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    Swap(other);
    return *this;
}

void Texture2D::LoadFromFile(const char* filePath)
{
    int texWidth, texHeight, texChannels;

    stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    CC_ASSERT(pixels, "Failed to load image file!")

    uint64_t imageSize = texWidth * texHeight * 4;

    uint8_t* pixelPtr = new uint8_t[imageSize];
    defer([pixelPtr] { delete[] pixelPtr; });

    // Set the image to 0 so that we get no garbage data in the pixel array
    memset(pixelPtr, 0, imageSize);
    memcpy(pixelPtr, pixels, sizeof(uint8_t) * imageSize);

    Buffer::Buffer stagingBuffer;
    Buffer::CreateBuffer(imageSize,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         // specifies that data is accessible on the CPU.
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             // Ensures that CPU and GPU memory are consistent across both devices.
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT stagingBuffer.buffer,
                         OUT stagingBuffer.bufferMemory);

    Buffer::CopyData(stagingBuffer, imageSize, pixelPtr);

    stbi_image_free(pixels);

    extent = {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)};

    Utils::Extent3D imageExtent {static_cast<uint32_t>(texWidth),
                                 static_cast<uint32_t>(texHeight),
                                 1};
    image = Image({Utils::RGBASRGB, imageExtent, Vulkan::Utils::USAGE_TEXTURE, 1, 1});

    image.CopyBuffer(stagingBuffer.buffer, imageExtent);

    Buffer::DestroyBuffer(stagingBuffer);
}

void Texture2D::LoadTexture(const uint8_t* pixels, size_t size, uint32_t width, uint32_t height, Usage texUsage)
{
    Buffer::Buffer stagingBuffer;
    Buffer::CreateBuffer(size,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         // specifies that data is accessible on the CPU.
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             // Ensures that CPU and GPU memory are consistent across both devices.
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT stagingBuffer.buffer,
                         OUT stagingBuffer.bufferMemory);

    Buffer::CopyData(stagingBuffer, size, pixels);

    extent = {width, height};

    Utils::Extent3D imageExtent {width, height, 1};

    image = Image({(Utils::ImageFormat)texUsage, imageExtent, Vulkan::Utils::USAGE_TEXTURE, 1, 1});

    image.CopyBuffer(stagingBuffer.buffer, imageExtent);

    Buffer::DestroyBuffer(stagingBuffer);
}

void Texture2D::CopyToRegion(uint8_t* pixels, unsigned long size, Utils::Extent3D copyExtent, Utils::Offset3D copyOffset)
{
    Buffer::Buffer stagingBuffer;
    defer([&stagingBuffer](){ Buffer::DestroyBuffer(stagingBuffer); });
    Buffer::CreateBuffer(size,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         // specifies that data is accessible on the CPU.
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             // Ensures that CPU and GPU memory are consistent across both devices.
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT stagingBuffer.buffer,
                         OUT stagingBuffer.bufferMemory);

    Buffer::CopyData(stagingBuffer, size, pixels);

    image.CopyBuffer(stagingBuffer.buffer, copyExtent,  copyOffset);
}

void Texture2D::Free()
{
    auto device = Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroySampler(device, info.sampler, nullptr);

    image.Free();

    info = {{}, nullptr};
}

void Texture2D::Swap(Texture2D& other)
{
    auto tmpId = id;
    auto tmpImage = std::move(image);
    auto tmpInfo = info;
    auto tmpExtent = extent;

    id = other.id;
    image = std::move(other.image);
    info = other.info;
    extent = other.extent;

    other.id = tmpId;
    other.image = std::move(tmpImage);
    other.info = tmpInfo;
    other.extent = tmpExtent;
}

} // namespace Siege::Vulkan