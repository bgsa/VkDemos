#ifndef GRAPHIC_PIPELINE_HEADER
#define GRAPHIC_PIPELINE_HEADER

#include "VkDemosHeader.h"
#include "Rasterizer.hpp"
#include "VkSwapChain.h"
#include "Shader.h"
#include "Viewport.h"
#include "Device.h"

namespace VkDemos
{
class GraphicPipeline
{
private:
  VkDevice device;
  VkPipelineLayout pipelineLayout;

public:
  VkPipeline graphicPipeline;

  GraphicPipeline(const Device *device, Shader *shader, VkSwapChain *swapChain, Viewport *viewport);
  ~GraphicPipeline();
};
} // namespace VkDemos

#endif