#include "ShaderManager.h"

namespace VkBootstrap
{

VkShaderModule *ShaderManager::createShaderModule(const VkDevice &device, const File *shaderFile)
{
    VkShaderModule *shaderModule = new VkShaderModule;

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderFile->length;
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderFile->content);

    VkResult operationResult = vkCreateShaderModule(device, &createInfo, nullptr, shaderModule);

    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module: " + VkHelper::getVkResultDescription(operationResult));

    return shaderModule;
}

}
