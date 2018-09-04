#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "VkBootstrapHeader.h"
#include "ShaderManager.h"
#include "Device.h"

namespace VkBootstrap
{
class Shader
{
private:
  VkDevice device = VK_NULL_HANDLE;
  VkShaderModule *vertexShaderModule = nullptr;
  VkShaderModule *fragmentShaderModule = nullptr;

public:
  static Shader *createShader(const Device *device, const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
  static Shader *createShader(const Device *device, const File *vertexShaderFile, const File *fragmentShaderFile);

  VkPipelineShaderStageCreateInfo getVertexPipelineShaderStageInfo();
  VkPipelineShaderStageCreateInfo getFragmentPipelineShaderStageInfo();
  VkPipelineShaderStageCreateInfo *getPipelinesShaderStageInfo();

  ~Shader();
};
}

#endif