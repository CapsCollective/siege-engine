//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Texture2D.h"
#include "Context.h"
#include "render/renderer/buffer/Buffer.h"
#include "utils/Descriptor.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <utils/Logging.h>

namespace Siege::Vulkan
{

Texture2D::Texture2D(const String& name, const String& filePath)
{
    int texWidth, texHeight, texChannels;

    stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    CC_ASSERT(pixels, "Failed to load image file!")

    uint64_t imageSize = texWidth * texHeight * 4;

    uint8_t pixelPtr[imageSize];

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

    Utils::Extent3D imageExtent {static_cast<uint32_t>(texWidth),
                                 static_cast<uint32_t>(texHeight),
                                 1};
    image = Image({Utils::RGBASRGB, imageExtent, Vulkan::Utils::USAGE_TEXTURE, 1, 1});

    image.CopyBuffer(stagingBuffer.buffer);

    Buffer::DestroyBuffer(stagingBuffer);

    VkSamplerCreateInfo samplerInfo = Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR);

    info = {image.GetInfo()};

    vkCreateSampler(Context::GetVkLogicalDevice(), &samplerInfo, nullptr, &info.sampler);

    id = INTERN_STR(name);
}

Texture2D::Texture2D(Texture2D&& other)
{
    Swap(other);
}

Texture2D::~Texture2D()
{
    auto device = Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroySampler(device, info.sampler, nullptr);

    info = {{}, nullptr};
}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    Swap(other);
    return *this;
}

void Texture2D::Swap(Texture2D& other)
{
    auto tmpId = id;
    auto tmpInfo = info;
    auto tmpImage = std::move(image);

    id = other.id;
    info = other.info;
    image = std::move(other.image);

    other.id = tmpId;
    other.info = tmpInfo;
    other.image = std::move(tmpImage);
}

} // namespace Siege