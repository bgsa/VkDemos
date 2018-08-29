#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "VkDemosHeader.h"
#include "ShaderManager.h"

namespace VkDemos
{
class Shader
{
private:
  const VkDevice *device = nullptr;
  VkShaderModule *vertexShaderModule = nullptr;
  VkShaderModule *fragmentShaderModule = nullptr;

public:
  static Shader *createShader(const VkDevice *device, const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
  static Shader *createShader(const VkDevice *device, const File *vertexShaderFile, const File *fragmentShaderFile);

  VkPipelineShaderStageCreateInfo getVertexPipelineShaderStageInfo();
  VkPipelineShaderStageCreateInfo getFragmentPipelineShaderStageInfo();
  VkPipelineShaderStageCreateInfo *getPipelinesShaderStageInfo();

  ~Shader();
};
} // namespace VkDemos

#endif