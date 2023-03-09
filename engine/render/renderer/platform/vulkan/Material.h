//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
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
     */
    Material(Shader vertShader, Shader fragShader);
    /**
     * A move constructor for the Material class
     * @param other the Material to be moved
     */
    Material(Material&& other) noexcept;

    /**
     * A destructor for the Material class
     */
    ~Material();

    void Destroy();

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
    void SetTexture(Hash::StringId id, uint32_t index, Texture2D* texture);

    /**
     * Binds the Material for rendering (also binds the stored Pipeline)
     * @param commandBuffer the command buffer being used to record rendering information
     */
    void Bind(const CommandBuffer& commandBuffer);

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
        VkDescriptorSetLayout layout {VK_NULL_HANDLE};
        MSArray<Property, 10> properties;
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

    /**
     * Writes every set in the provided array
     * @param sets an array of all sets to write to
     */
    void Write(MSArray<VkWriteDescriptorSet, 10>& sets);

    /**
     * Queues an image update to be written to at the end of the frame
     * @param writeQueue the collection of write sets to queue into
     * @param set the descriptor set to be written to
     * @param binding the binding to write to
     * @param count the number of descriptors to update
     * @param index the index to start from
     */
    void QueueImageUpdate(MSArray<VkWriteDescriptorSet, 10>& writeQueue,
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
    void QueuePropertyUpdate(MSArray<VkWriteDescriptorSet, 10>& writeQueue,
                             VkDescriptorSet& set,
                             Utils::UniformType type,
                             uint32_t binding,
                             uint32_t count);

    /**
     * Checks if a descriptor is a 2D texture
     * @param type the property's type
     * @return true if the type is a Texture2D, false if it isn't
     */
    inline bool IsTexture2D(Utils::UniformType type)
    {
        return type == Utils::TEXTURE2D;
    }

    Pipeline graphicsPipeline;

    Shader vertexShader;
    Shader fragmentShader;

    uint64_t bufferSize {0};
    Buffer::Buffer buffer;

    // TODO: Can probably bundle this into a struct?
    MSArray<PropertiesSlot, MAX_UNIFORM_SETS> propertiesSlots;

    // Descriptor set data
    MSArray<VkDescriptorBufferInfo, MAX_UNIFORM_SETS * 10> bufferInfos;
    MHArray<MSArray<VkDescriptorSet, MAX_UNIFORM_SETS>> perFrameDescriptorSets;
    MHArray<MSArray<VkWriteDescriptorSet, 10>> writes;

    // Texture data
    MSArray<VkDescriptorImageInfo, MAX_TEXTURES> texture2DInfos;
    MSArray<Hash::StringId, MAX_TEXTURES> textureIds;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_MATERIAL_H
