#ifndef SHADER_MANAGER_HEADER
#define SHADER_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "File.h"

namespace VkDemos
{
class ShaderManager
{
public:
  static VkShaderModule *createShaderModule(const VkDevice &device, const File *shaderFile);
};
} // namespace VkDemos

#endif