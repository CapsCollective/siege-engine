//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Font.h"
#include "Context.h"
#include "utils/TypeAdaptor.h"
#include "render/renderer/buffer/Buffer.h"
#include "utils/Descriptor.h"
#include FT_FREETYPE_H

#include <utils/Logging.h>
#include <utils/Defer.h>

namespace Siege::Vulkan
{

FT_Library Font::freetype = nullptr;

Font::Font(const char* filePath)
{
    auto device = Context::GetCurrentDevice();

    CC_ASSERT(!FT_New_Face(freetype, filePath, 0, &fontFace), "Failed to load font!")

    Utils::Extent3D imageExtent {width, height, 1};

    image = Image({Utils::RED8UN, imageExtent, Utils::USAGE_TEXTURE, 1, 1});

    uint8_t* buffer = new uint8_t[width*height];
    defer([buffer](){ delete [] buffer; });

    memset(buffer, 0, sizeof(uint8_t) * width * height);

    Buffer::Buffer stagingBuffer;

    Buffer::CreateBuffer(sizeof(uint8_t) * width * height,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         // specifies that data is accessible on the CPU.
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             // Ensures that CPU and GPU memory are consistent across both devices.
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT stagingBuffer.buffer,
                         OUT stagingBuffer.bufferMemory);

    Buffer::CopyData(stagingBuffer, sizeof(uint8_t) * width * height, buffer);

    image.TransitionLayout(Utils::STAGE_TRANSFER_BIT, Utils::LAYOUT_TRANSFER_DST_OPTIMAL, Utils::ACCESS_TRANSFER_WRITE);

    CommandBuffer::ExecuteSingleTimeCommand([&](VkCommandBuffer commandBuffer) {
        VkBufferImageCopy copyRegion = {};
        copyRegion.bufferOffset = 0;
        copyRegion.bufferRowLength = 0;
        copyRegion.bufferImageHeight = 0;

        copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount = 1;
        copyRegion.imageOffset = {0, 0, 0};
        copyRegion.imageExtent = {width, height, 1};

        vkCmdCopyBufferToImage(commandBuffer,
                               stagingBuffer.buffer,
                               image.GetImage(),
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &copyRegion);
    });

    Buffer::DestroyBuffer(stagingBuffer);

    image.TransitionLayout(Utils::STAGE_FRAGMENT_SHADER, Utils::LAYOUT_SHADER_READ_ONLY_OPTIMAL, Utils::ACCESS_SHADER_READ);

    VkSamplerCreateInfo samplerInfo =
        Utils::Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

    vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &info.sampler);

    id = INTERN_STR(filePath);

    info.imageInfo = image.GetInfo();
}

Font::~Font()
{
    Free();
}

void Font::Free()
{
    auto device = Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroySampler(device, info.sampler, nullptr);

    image.Free();

    info = {{}, nullptr};
}

void Font::Swap(Font& other)
{
    auto tmpFontFace = fontFace;
    auto tmpId = id;
    auto tmpWidth = width;
    auto tmpHeight = height;
    auto tmpImage = std::move(image);
    auto tmpInfo = other.info;

    fontFace = other.fontFace;
    id = other.id;
    width = other.width;
    height = other.height;
    image = std::move(other.image);
    info = other.info;

    other.fontFace = tmpFontFace;
    other.id = tmpId;
    other.width = tmpWidth;
    other.height = tmpHeight;
    other.image = std::move(tmpImage);
    other.info = tmpInfo;
}

void Font::InitialiseFontLibs()
{
    CC_ASSERT(!FT_Init_FreeType(&freetype), "Failed to initialise freetype!")
}

void Font::DestroyFontLibs()
{
    FT_Done_FreeType(freetype);
}

} // namespace Siege