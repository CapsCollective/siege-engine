//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_SHADER_H
#define SIEGE_ENGINE_VULKAN_SHADER_H

#include <utils/String.h>
#include <utils/collections/HeapArray.h>
#include <utils/collections/StackArray.h>

#include "Texture2D.h"
#include "render/renderer/buffer/Buffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Shader class stores all data related to Shaders including vertex, uniform, and storage shader
 * inputs. This class is specific to Vulkan and stores all Vulkan related data (specifically the
 * VkShaderModule). NOTE: This type is only compatible with .spv files
 */
class Shader
{
public:

    /**
     * Stores all data related to uniform types (uniform and storage buffers).
     * TODO: Add the ability to specify a push constant?
     *
     * The Uniform holds the following parameters:
     * @param id a hashed representation of the uniform name
     * @param totalSize the total size of the uniform (in bytes)
     * @param set the descriptor set the uniform belongs to
     * @param slot the binding location of the uniform
     * @param type the type of the uniform (texture, storage, or uniform)
     */
    struct Uniform
    {
        Hash::StringId id {0};
        uint32_t totalSize {0};
        uint32_t set {0};
        uint32_t slot {0};
        uint32_t count {0};
        Utils::UniformType type {Utils::UNKNOWN};
    };

    /**
     * A struct representing a shader vertex input. Stores memory alignment data related to a single
     * vertex attribute
     *
     * @param offset the offset of the attribute within a struct (assuming all vertices are stored
     * within a struct)
     * @param type the type of attribute (currently only supports Vector types)
     */
    struct VertexAttribute
    {
        uint32_t offset {0};
        Utils::VertexAttributeType type {Utils::VertexAttributeType::VERTEX_UNDEFINED};
        Utils::VertexInputRate inputRate;
    };

    /**
     * A struct representing a set of shader vertex inputs. Stores all vertex data that a shader
     * requires along with memory alignment data
     *
     * @param MAX_VERTEX_ATTRIBUTES specifies how many vertex attributes can be handled per vertex
     * shader
     * @param attributes a list of vertex attributes stored by the binding
     * @param stride the total stride of all vertices stored on this binding
     */
    struct VertexBinding
    {
    public:

        VertexBinding& WithInputRate(Utils::VertexInputRate rate);
        /**
         * Specifies a 3D vector vertex attribute
         * @return a reference to the VertexBinding type
         */
        VertexBinding& AddFloatVec3Attribute();

        /**
         * Specifies a 4D vector vertex attribute
         * @return a reference to the VertexBinding type
         */
        VertexBinding& AddFloatVec4Attribute();

        /**
         * Specifies a 2D vector vertex attribute
         * @return a reference to the VertexBinding type
         */
        VertexBinding& AddFloatVec2Attribute();

        VertexBinding& AddU32Attribute();

        VertexBinding& AddMat4Attribute();

        VertexBinding& AddMat3Attribute();

        VertexBinding& AddMat2Attribute();

        static constexpr uint64_t MAX_VERTEX_ATTRIBUTES {10};
        MSArray<VertexAttribute, MAX_VERTEX_ATTRIBUTES> attributes;
        Utils::VertexInputRate inputRate {Utils::VertexInputRate::INPUT_RATE_VERTEX};
        uint32_t stride {0};
    };

    struct PushConstant
    {
        uint32_t size {0};
    };

    /**
     * A helper struct for configuring a shader. Contains helper methods to easily string together
     * shader configuration functions. Makes shader creation a little more streamlined and easier to
     * parse.
     *
     * @param filePath the filePath of the shader (must be a .spv shader file)
     * @param expectedTopology the expected vertex topology for the vertex shader (triangle list,
     * line list...etc)
     * @param type the shader type (currently only supporting vertex and fragment shaders)
     * @param uniforms a list of all uniforms expected by the shader
     * @param vertexBindings a list of all vertex bindings expected by the shader
     * @param totalUniformSize the total size (in bytes) of the shader
     * @param attributeCount the number of vertex attributes expected by the shader
     */
    struct Builder
    {
        /**
         * Specifies which vertex shader to use (is overridden if FromFragmentShader is called)
         * @param path the file path to the vertex shader (must be a .spv shader file)
         * @return a reference to the current Builder object
         */
        Builder& FromVertexShader(const String& path);

        /**
         * Specifies which fragment shader to use (is overridden if FromVertexShader is called)
         * @param path the file path to the fragment shader (must be a .spv shader file)
         * @return a reference to the current Builder object
         */
        Builder& FromFragmentShader(const String& path);

        /**
         * Adds a vertex binding to the shader
         * @param binding a configured VertexBinding struct
         * @return a reference to the current Builder object
         */
        Builder& WithVertexBinding(const VertexBinding& binding);

        /**
         * Specifies a uniform type to be added to the shader
         * @tparam T the type of uniform (i.e: the data type being loaded to the GPU)
         * @param name the name of the uniform
         * @param set the descriptorSet the uniform is stored in
         * @param size the number of elements in the uniform (if passing in an array)
         * @param count the number of dynamic uniforms stored
         * @return a reference to the current Builder object
         */
        template<typename T>
        inline Builder& WithUniform(const String& name,
                                    uint32_t set = 0,
                                    uint64_t size = 1,
                                    uint32_t count = 1)
        {
            auto bufferSize = sizeof(T);

            uniforms.Append({INTERN_STR(name.Str()),
                             static_cast<uint32_t>(bufferSize * size),
                             set,
                             static_cast<uint32_t>(uniforms.Count()),
                             count,
                             Utils::UNIFORM});

            totalUniformSize += (bufferSize * size) * count;

            return *this;
        }

        /**
         * Specifies a storage type to be added to the shader
         * @tparam T the type of storage buffer (i.e: the data type being loaded to the GPU)
         * @param name the name of the storage buffer
         * @param set the descriptorSet the storage buffer is stored in
         * @param size the number of elements in the storage buffer (if passing in an array)
         * @param count the number of dynamic storage buffers stored
         * @return a reference to the current Builder object
         */
        template<typename T>
        inline Builder& WithStorage(const String& name,
                                    uint32_t set = 0,
                                    uint64_t size = 1,
                                    uint32_t count = 1)
        {
            auto bufferSize = sizeof(T);

            uniforms.Append({INTERN_STR(name.Str()),
                             static_cast<uint32_t>(bufferSize * size),
                             set,
                             static_cast<uint32_t>(uniforms.Count()),
                             count,
                             Utils::STORAGE});

            totalUniformSize += (bufferSize * size) * count;

            return *this;
        }

        /**
         * Adds a uniform type with Global 3D per-scene data. In this case, the data stores lighting
         * and camera information
         * @param set the descriptorSet the uniform is stored in
         * @return a reference to the current Builder object
         */
        Builder& WithGlobalData3DUniform(uint32_t set = 0);

        /**
         * Adds a storage buffer type with all configured 3D transforms
         * @param set the descriptorSet the storage buffer is stored in
         * @param size the number of elements in the buffer
         * @return a reference to the current Builder object
         */
        Builder& WithTransform3DStorage(uint32_t set = 0, uint64_t size = 1);

        /**
         * Adds a storage buffer type with all configured 2D transforms
         * @param set the descriptorSet the storage buffer is stored in
         * @param size the number of elements in the buffer
         * @return a reference to the current Builder object
         */
        Builder& WithTransform2DStorage(uint32_t set = 0, uint64_t size = 1);

        Builder& WithPushConstant(uint64_t size);

        /**
         * Specifies the expected vertex topology (triangle list, line list...etc)
         * @param topology an enum specifying the vertex topology
         * @return a reference to the current Builder object
         */
        Builder& WithVertexTopology(Utils::PipelineTopology topology);

        Builder& WithTexture(const String& name, uint32_t set = 1, uint32_t count = 1);

        Builder& WithDefaultTexture(const Texture2D* texture);

        /**
         * Builds a Shader and returns it (destroys the Builder in the process)
         * @return a new Shader with all configured data within
         */
        Shader Build() const;

        String filePath {""};
        Utils::PipelineTopology expectedTopology {Utils::PipelineTopology::TOPOLOGY_TRIANGLE_LIST};
        Utils::ShaderType type {Utils::ALL_GRAPHICS};
        MSArray<Uniform, 10> uniforms;
        MSArray<VertexBinding, 5> vertexBindings;
        Texture2D::Info defaultTextureInfo;
        PushConstant pushConstant;
        uint64_t totalUniformSize {0};
        uint32_t attributeCount {0};
    };

    /**
     * A default constructor for the Shader (leaves everything empty)
     */
    Shader() = default;

    /**
     * A copy constructor between two shaders
     * @param other the shader to copy data from
     */
    Shader(const Shader& other);

    /**
     * A full-parameter constructor for the Shader.
     * @param filePath the filePath of the shader (must be a .spv shader file)
     * @param expectedTopology the expected vertex topology for the vertex shader (triangle list,
     * line list...etc)
     * @param type the shader type (currently only supporting vertex and fragment shaders)
     * @param uniforms a list of all uniforms expected by the shader
     * @param vertexBindings a list of all vertex bindings expected by the shader
     * @param totalUniformSize the total size (in bytes) of the shader
     * @param attributeCount the number of vertex attributes expected by the shader
     */
    Shader(const String& filePath,
           Utils::ShaderType type,
           Utils::PipelineTopology expectedTopology,
           MSArray<Uniform, 10> uniforms,
           MSArray<VertexBinding, 5> vertices,
           Texture2D::Info tex2DInfo,
           PushConstant pushConstant,
           size_t totalSize,
           uint32_t totalVertexAttributes);

    /**
     * A move constructor for the Shader
     * @param other the Shader to be moved
     */
    inline Shader(Shader&& other)
    {
        Swap(other);
    }

    /**
     * A destructor for the Shader
     */
    ~Shader();

    void Destroy();

    static MHArray<char> ReadFileAsBinary(const String& filePath);

    /**
     * A move assignment operator
     * @param other the Shader to move
     * @return a reference to the current Shader
     */
    inline Shader& operator=(Shader&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    /**
     * A copy assignment operator
     * @param other the Shader to be copied
     * @return a reference to the new Shader
     */
    inline Shader& operator=(const Shader& other) noexcept
    {
        filePath = other.filePath;

        CreateShaderModule();

        type = other.type;
        expectedUniforms = other.expectedUniforms;
        vertexBindings = other.vertexBindings;
        totalUniformSize = other.totalUniformSize;
        defaultTextureInfo = other.defaultTextureInfo;
        pushConstant = other.pushConstant;

        return *this;
    }

    /**
     * Returns all vertex bindings held by the Shader
     * @return a StackArray containing all VertexBindings
     */
    inline const MSArray<VertexBinding, 5>& GetVertexBindings() const
    {
        return vertexBindings;
    }

    inline const MSArray<Uniform, 10>& GetUniforms() const
    {
        return expectedUniforms;
    }

    /**
     * Returns the Vulkan ShaderModule for the Shader
     * @return A VkShaderModule type
     */
    inline const VkShaderModule GetShaderModule() const
    {
        return shaderModule;
    }

    /**
     * Returns the total number of vertex attributes expected by the shader
     * @return the total number of vertices expected by the shader
     */
    inline const uint32_t GetTotalAttributeCount() const
    {
        return totalVertexAttributeCount;
    }

    /**
     * Returns the topology of the vertices (i.e: triangle list, point list...etc)
     * @return The vertex topology expected by the shader
     */
    inline const Utils::PipelineTopology GetVertexTopology() const
    {
        return expectedTopology;
    }

    /**
     * Returns the type of shader (Vertex or Fragment)
     * @return the type of the shader
     */
    inline Utils::ShaderType GetShaderType() const
    {
        return type;
    }

    inline const Texture2D::Info& GetDefaultTexture2DInfo() const
    {
        return defaultTextureInfo;
    }

    inline Texture2D::Info& GetDefaultTexture2DInfo()
    {
        return defaultTextureInfo;
    }

    inline const PushConstant GetPushConstant() const
    {
        return pushConstant;
    }

    inline const bool HasPushConstant() const
    {
        return pushConstant.size > 0;
    }

private:

    /**
     * Swaps the values between two shaders (used for moving)
     * @param other the Shader to swap with
     */
    void Swap(Shader& other);

    /**
     * Creates the Vulkan Shader Module
     * TODO: Move this to a Vulkan renderer which can store this information
     */
    void CreateShaderModule();

    String filePath {""};
    Utils::ShaderType type {Utils::ShaderType::EMPTY};
    Utils::PipelineTopology expectedTopology {Utils::PipelineTopology::TOPOLOGY_TRIANGLE_LIST};
    VkShaderModule shaderModule {nullptr};
    MSArray<Uniform, 10> expectedUniforms;
    MSArray<VertexBinding, 5> vertexBindings;
    Texture2D::Info defaultTextureInfo {};
    PushConstant pushConstant;
    size_t totalUniformSize {0};
    uint32_t totalVertexAttributeCount {0};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_SHADER_H
