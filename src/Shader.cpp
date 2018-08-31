#include "Shader.h"

namespace VkDemos
{

Shader *Shader::createShader(const Device *device, const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
    File *vertexShaderFile = File::readFile(vertexShaderFilename);
    File *fragmentShaderFile = File::readFile(fragmentShaderFilename);

    Shader *shader = createShader(device, vertexShaderFile, fragmentShaderFile);

    delete vertexShaderFile;
    delete fragmentShaderFile;

    return shader;
}

Shader *Shader::createShader(const Device *device, const File *vertexShaderFile, const File *fragmentShaderFile)
{
    Shader *shader = new Shader;

    shader->device = device->logicalDevice;
    shader->vertexShaderModule = ShaderManager::createShaderModule(device->logicalDevice, vertexShaderFile);
    shader->fragmentShaderModule = ShaderManager::createShaderModule(device->logicalDevice, fragmentShaderFile);

    return shader;
}

VkPipelineShaderStageCreateInfo Shader::getVertexPipelineShaderStageInfo()
{
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = *vertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    return vertexShaderStageInfo;
}

VkPipelineShaderStageCreateInfo Shader::getFragmentPipelineShaderStageInfo()
{
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = *fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    return fragmentShaderStageInfo;
}

VkPipelineShaderStageCreateInfo *Shader::getPipelinesShaderStageInfo()
{
    VkPipelineShaderStageCreateInfo *shaderStages = new VkPipelineShaderStageCreateInfo[2];
    shaderStages[0] = getVertexPipelineShaderStageInfo();
    shaderStages[1] = getFragmentPipelineShaderStageInfo();

    return shaderStages;
}

Shader::~Shader()
{
    if (vertexShaderModule != nullptr)
    {
        vkDestroyShaderModule(device, *vertexShaderModule, nullptr);
        vertexShaderModule = nullptr;
    }

    if (fragmentShaderModule != nullptr)
    {
        vkDestroyShaderModule(device, *fragmentShaderModule, nullptr);
        fragmentShaderModule = nullptr;
    }
}

} // namespace VkDemos