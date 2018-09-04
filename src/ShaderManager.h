#ifndef SHADER_MANAGER_HEADER
#define SHADER_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "File.h"

namespace VkBootstrap
{
class ShaderManager
{
public:
  static VkShaderModule *createShaderModule(const VkDevice &device, const File *shaderFile);
};
}

#endif