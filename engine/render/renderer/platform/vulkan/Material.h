//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_MATERIAL_H
#define SIEGE_ENGINE_VULKAN_MATERIAL_H

#include "CommandBuffer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Texture2D.h"

namespace Siege::Vulkan
{
/**
 * The Material class represents a Material in Vulkan. The class is responsible for converting
 * shader uniforms into Vulkan descriptors and manage the rendering pipeline for the material
 *
 * TODO: Maybe move the rendering pipeline to be separate and just hold a reference to the pipeline?
 *
 * @param graphicsPipeline the Vulkan pipeline stored in the material
 * @param vertexShader the vertex shader used by the material
 * @param fragmentShader the fragment shader used by the material
 * @param bufferSize the total size of the memory allocated for this material on the GPU
 * @param buffer the buffer holding all uniform data
 * @param propertiesSlots an array storing all uniforms the material knows about
 * @param perFrameDescriptorSets an array holding all descriptor sets the Material uses. Holds one
 * per frames in flight
 * @param bufferInfos an array holding buffer access information
 * @param writes an array holding uniform update structs. These are cached to avoid potentially
 * remaking every them every frame
 * @param texture2DInfos a list of texture 2D rendering infos used in textures
 * @param textureIds a list of texture IDs (used for checking the existence of textures)
 * @param isWritingDepth a boolean specifying if the material pipeline is writing to the depth
 * buffer
 */
class Material
{
public:

    static constexpr uint32_t MAX_UNIFORM_SETS = 2;
    static constexpr uint32_t MAX_TEXTURES = 16;
    /**
     * An empty default constructor for the Material class
     */
    Material() = default;
    /**
     * A constructor for the material which expects a vertex and fragment shader at minimum
     * @param vertShader the vertex shader to be used by the Material
     * @param fragShader the fragment shader to be used by the Material
     * @param isWritingDepth specifies if the material pipeline is writing to the depth buffer
     */
    Material(Shader vertShader, Shader fragShader, bool isWritingDepth = true);
    /**
     * A move constructor for the Material class
     * @param other the Material to be moved
     */
    Material(Material&& other) noexcept;

    /**
     * A destructor for the Material class
     */
    ~Material();

    /**
     * Frees the memory held by the material
     */
    void Free();

    /**
     * A movement assignment operator
     * @param other the Material to be moved
     * @return a reference to the new Material
     */
    Material& operator=(Material&& other);

    /**
     * Sets the uniform to the provided data. Uses a string ID to avoid string comparison
     * @param id the ID of the uniform
     * @param dataSize the size of the memory being allocated to the uniform
     * @param data the data to be inputted into the uniform
     */
    void SetUniformData(Hash::StringId id, uint64_t dataSize, const void* data);

    /**
     * TODO(Aryeh): Document this
     * @param id
     * @param index
     * @param textureInfo
     */
    uint32_t SetTexture(Hash::StringId id, Texture2D* texture);

    /**
     * Binds the Material for rendering (also binds the stored Pipeline)
     * @param commandBuffer the command buffer being used to record rendering information
     */
    void Bind(const CommandBuffer& commandBuffer);

    void BindPushConstant(const CommandBuffer& commandBuffer, const void* values);

    /**
     *  Recreates the Material pipeline (used primarily for window size changes)
     */
    void Recreate();

    /**
     *  Updates the descriptors stored in the Material
     */
    void Update();

    /**
     * Updates the specified descriptor using the uniform name/string ID
     * @param id the name of the uniform
     */
    void Update(Hash::StringId id);

private:

    /**
     * The Property struct stores information about a uniform (specifically used for configuring
     * descriptors)
     *
     * @param id the unique identifier for the property
     * @param binding the binding within the descriptor set
     * @param offset the memory offset of the property within the material buffer
     * @param size the total size of the property
     * @param type the type of Property (Uniform, Storage...etc)
     * @param shaderStage the shader stages where the uniform is used
     */
    struct Property
    {
        Hash::StringId id {0};
        uint32_t binding {0};
        uint32_t count {0};
        uint64_t offset {0};
        uint64_t size {0};
        Utils::UniformType type {Utils::UNKNOWN};
        Utils::ShaderType shaderStage {Utils::EMPTY};
    };

    /**
     *  Specifies a slot within the shader where the descriptor set is stored. Effectively acts as
     *  descriptor set abstraction
     *
     *  @param layout the attribute layout of the slot (represented as a vulkan DescriptorSetLayout)
     *  @param properties the individual uniforms stored in the slot (the descriptors in Vulkan
     * terminology)
     */
    struct PropertiesSlot
    {
        VkDescriptorSetLayout layout {nullptr};
        MSArray<Property, 10> properties;
    };

    /**
     * A struct specifying a push constant type
     */
    struct PushConstant
    {
        uint32_t size {0};
        Utils::ShaderType type {Utils::EMPTY};
    };

    struct UniformUpdate
    {
        VkDescriptorSet set {nullptr};
        unsigned int dstBinding {0};
        unsigned int dstIndex {0};
        unsigned int descriptors {0};
        Utils::UniformType type;
    };

    struct ImageData
    {
        VkSampler sampler {nullptr};
        VkImageView view {nullptr};
        Utils::ImageLayout layout {Utils::LAYOUT_UNDEFINED};
    };

    struct UniformImageUpdate
    {
        UniformUpdate update;
        MHArray<ImageData> imageUpdates;
    };

    struct BufferData
    {
        VkBuffer buffer;
        unsigned long offset;
        unsigned long range;
    };

    struct UniformBufferUpdate
    {
        UniformUpdate update;
        BufferData bufferUpdate;
    };

    /**
     * Finds a Property's index based on a provided ID
     * @param id the ID to search for
     * @param slot the slot to iterate through
     * @return the index of the property, or -1 if the property wasn't found
     */
    int32_t FindPropertyIndex(Hash::StringId id, PropertiesSlot& slot);

    /**
     * Finds a Texture's index based on a provided ID
     * @param id the ID to search for
     * @param slot the slot to iterate through
     * @return the index of the texture, or -1 if the property wasn't found
     */
    int32_t FindTextureIndex(Hash::StringId id);

    /**
     * Swaps the contents of two Materials
     * @param other the Material to swap values with
     */
    void Swap(Material& other);
    /**
     * Adds a shader to the Material and adds all shader uniforms to the Material
     * @param shader the shader to be added
     * @param offset a reference to the memory offset that the shader data will be stored at
     */
    void AddShader(const Shader& shader, uint64_t& offset);
    /**
     * Writes the values of the buffer to the descriptor sets within the GPU
     * @param set the descriptor set index
     * @param slot the slot to be written to
     */
    void WriteSet(uint32_t set, PropertiesSlot& slot);

    void UpdateUniforms(MHArray<UniformBufferUpdate>& updates, MHArray<UniformImageUpdate>& images);

    /**
     * Queues an image update to be written to at the end of the frame
     * @param writeQueue the collection of write sets to queue into
     * @param set the descriptor set to be written to
     * @param binding the binding to write to
     * @param count the number of descriptors to update
     * @param index the index to start from
     */
    void QueueImageUpdate(MHArray<UniformImageUpdate>& imageUpdate,
                          VkDescriptorSet& set,
                          uint32_t binding,
                          uint32_t count = MAX_TEXTURES,
                          uint32_t index = 0);

    /**
     * Queues a generic property update to be written to at the end of the frame
     * @param writeQueue the collection of write sets to queue into
     * @param set the descriptor set to be written to
     * @param type the type of descriptor tio update
     * @param binding the binding to write to
     * @param count the number of descriptors to update
     */
    void QueuePropertyUpdate(MHArray<UniformBufferUpdate>& bufferUpdateQueue,
                             VkDescriptorSet& set,
                             Utils::UniformType type,
                             uint32_t binding,
                             uint32_t count,
                             BufferData bufferInfo);

    /**
     * Checks if a descriptor is a 2D texture
     * @param type the property's type
     * @return true if the type is a Texture2D, false if it isn't
     */
    inline bool IsTexture2D(Utils::UniformType type)
    {
        return type == Utils::TEXTURE2D;
    }

    /**
     * Checks if a descriptor is a uniform
     * @param type the property's type
     * @return true if the type is a Texture2D, false if it isn't
     */
    inline bool IsUniform(Utils::UniformType type)
    {
        return type == Utils::UNIFORM;
    }

    /**
     * Checks if a descriptor is a storage uniform
     * @param type the property's type
     * @return true if the type is a Texture2D, false if it isn't
     */
    inline bool IsStorage(Utils::UniformType type)
    {
        return type == Utils::STORAGE;
    }

    Pipeline graphicsPipeline;

    Shader vertexShader;
    Shader fragmentShader;

    uint64_t bufferSize {0};
    Buffer::Buffer buffer;

    PushConstant pushConstant;

    MSArray<PropertiesSlot, MAX_UNIFORM_SETS> propertiesSlots;

    // Descriptor set data
    MHArray<MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>> perFrameDescriptorSets;

    MHArray<MHArray<UniformBufferUpdate>> bufferUpdates;
    MHArray<MHArray<UniformImageUpdate>> imageUpdates;
    MHArray<ImageData> textureInfos;

    // Texture data
    MSArray<Hash::StringId, MAX_TEXTURES> textureIds;

    bool isWritingDepth {true};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_MATERIAL_H
