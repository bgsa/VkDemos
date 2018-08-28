#include "Shader.h"

namespace VkDemos
{

Shader *Shader::createShader(const VkDevice *device, const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
    File *vertexShaderFile = File::readFile(vertexShaderFilename);
    File *fragmentShaderFile = File::readFile(fragmentShaderFilename);

    Shader *shader = createShader(device, vertexShaderFile, fragmentShaderFile);

    delete vertexShaderFile;
    delete fragmentShaderFile;

    return shader;
}

Shader *Shader::createShader(const VkDevice *device, const File *vertexShaderFile, const File *fragmentShaderFile)
{
    Shader *shader = new Shader;

    shader->device = device;
    shader->vertexShaderModule = ShaderManager::createShaderModule(device, vertexShaderFile);
    shader->fragmentShaderModule = ShaderManager::createShaderModule(device, vertexShaderFile);

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = *shader->vertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = *shader->fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

    return shader;
}

Shader::~Shader()
{
    if (vertexShaderModule != nullptr)
    {
        vkDestroyShaderModule(*device, *vertexShaderModule, nullptr);
        vertexShaderModule = nullptr;
    }

    if (fragmentShaderModule != nullptr)
    {
        vkDestroyShaderModule(*device, *fragmentShaderModule, nullptr);
        fragmentShaderModule = nullptr;
    }
}

} // namespace VkDemos