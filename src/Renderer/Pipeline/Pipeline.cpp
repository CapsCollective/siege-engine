#include "Pipeline.h"
#include "../Model/Model.h"

#include <fstream>
#include <string>
#include <iostream>

namespace SnekVk 
{
    Pipeline::Pipeline(
        VulkanDevice& device, 
        const char* vertFilePath, 
        const char* fragFilePath, 
        const PipelineConfigInfo& configInfo) : device{device}
    {
        CreateGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    Pipeline::Pipeline(VulkanDevice& device) : device{device}
    {}

    Pipeline::~Pipeline() 
    {
        ClearPipeline();
    }

    Utils::Array<char> Pipeline::ReadFile(const char* filePath)
    {
        // Read the file as binary and consume the entire file. 
        std::ifstream file { filePath, std::ios::ate | std::ios::binary };

        SNEK_ASSERT(file.is_open(), std::string("Could not find file: ") + filePath);

        // Since we consumed the entire file, we can tell the size by checking where
        // the file stream is reading from (which presumably is at the end of the file).
        u32 size = static_cast<u32>(file.tellg());

        Utils::Array<char> buffer(size);
        
        // Move to the beginning of the file. 
        file.seekg(0);
        
        file.read(buffer.Data(), size);

        file.close();
        
        return buffer;
    }

    void Pipeline::CreateGraphicsPipeline(
        char const* vertFilePath, 
        char const* fragFilePath, 
        const PipelineConfigInfo& configInfo)
    {

        SNEK_ASSERT(configInfo.pipelineLayout != VK_NULL_HANDLE, 
                "Cannot create graphics pipeline: no pipeline config provided in configInfo");
        
        SNEK_ASSERT(configInfo.renderPass != VK_NULL_HANDLE, 
                "Cannot create graphics pipeline: no renderpass config provided in configInfo");

        auto vertCode = ReadFile(vertFilePath);
        auto fragCode = ReadFile(fragFilePath);

        CreateShaderModule(vertCode, OUT &vertShader);
        CreateShaderModule(fragCode, OUT &fragShader);

        // We only support the vertex and fragment stages atm. Each stage is represented
        // by a PipelineShaderStageCreateInfo struct. 
        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT; // vertex stage of the pipeline
        shaderStages[0].module = vertShader;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT; // fragment stage of the pipeline
        shaderStages[1].module = fragShader;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        // In order to pass in vertex information, we must assign a set of descriptions to the shader.
        // These descriptions detail all data binding and which locations these bindings must be set to. 
        // In our case, we have a generic set of descriptions for our Vertex structs. 

        // TODO: Maybe inject these into the config. Right now the pipeline is highly coupled 
        // to the models. 
        auto bindingDescriptions = Model::Vertex::GetBindingDescriptions();
        auto attributeDescriptions = Model::Vertex::GetAttributeDescriptions();

        // Bind the descriptions to the pipeline to allow us to pass in vertex info via 
        // vertex buffers.
        VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
        vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
        vertexInputCreateInfo.vertexBindingDescriptionCount = static_cast<u32>(bindingDescriptions.size());
        vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        // Pass in all pipeline config details to the pipeline create info struct. 
        VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
        pipelineCreateInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineCreateInfo.pViewportState = &configInfo.viewportInfo;
        pipelineCreateInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineCreateInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineCreateInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineCreateInfo.pDynamicState = &configInfo.dynamicStateInfo;
        pipelineCreateInfo.pDepthStencilState = &configInfo.depthStencilInfo;

        pipelineCreateInfo.layout = configInfo.pipelineLayout;
        pipelineCreateInfo.renderPass = configInfo.renderPass;
        pipelineCreateInfo.subpass = configInfo.subPass;

        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;

        SNEK_ASSERT(vkCreateGraphicsPipelines(device.Device(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, OUT &graphicsPipeline) 
            == VK_SUCCESS, "Failed to create graphics pipeline!")
    }

    void Pipeline::RecreatePipeline(
        const char* vertFilePath, 
        const char* fragFilePath, 
        const PipelineConfigInfo& configInfo)
    {
        CreateGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    void Pipeline::ClearPipeline()
    {
        // TODO: Maybe we can get away without destroying the shader modules when 
        // re-creating the pipeline?
        vkDestroyShaderModule(device.Device(), vertShader, nullptr);
        vkDestroyShaderModule(device.Device(), fragShader, nullptr);
        
        vkDestroyPipeline(device.Device(), graphicsPipeline, nullptr);
    }

    void Pipeline::CreateShaderModule(Utils::Array<char>& fileData, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fileData.Size();
        // Because the code is expected to be numerical, we need to cast the values in the 
        // array to 32-bit unsigned integers. 
        createInfo.pCode = reinterpret_cast<const u32*>(fileData.Data());

        SNEK_ASSERT(vkCreateShaderModule(device.Device(), &createInfo, nullptr, OUT shaderModule) == VK_SUCCESS, 
            "Failed to create shader module!");
    }

    void Pipeline::Bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }

    PipelineConfigInfo Pipeline::DefaultPipelineConfig()
    {
        PipelineConfigInfo configInfo {};

        // Input Assembly specifies how vertices are interpreted in the pipeline. 
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        // Here we specify that all vertices should be interpreted as a list of triangles. Every three
        // vertices will therefore be considered to be attached as a triangle.
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportInfo.viewportCount = 1;
        configInfo.viewportInfo.pViewports = nullptr;
        configInfo.viewportInfo.scissorCount = 1;
        configInfo.viewportInfo.pScissors = nullptr;

        // Defines how we handle rasterisation for our vertices. 
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;       
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL; // fill the space between vertices.
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // clockwise front-face. 
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

        // Defines how multi-sampling is handled. 
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;
        configInfo.multisampleInfo.pSampleMask = nullptr;
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

        // Configures that types of colors we handle. In our case we allow a 4-part vector containing
        // an R,G,B, and A value. 
        configInfo.colorBlendAttachment.colorWriteMask = 
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;    
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        // Specifies further configurations for color blending
        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; 
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

        // Determines how image depth is handled
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f; 
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};
        configInfo.depthStencilInfo.back = {};

        configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.Data();
        configInfo.dynamicStateInfo.dynamicStateCount = static_cast<u32>(configInfo.dynamicStateEnables.Size());
        configInfo.dynamicStateInfo.flags = 0;

        return configInfo;
    }
}