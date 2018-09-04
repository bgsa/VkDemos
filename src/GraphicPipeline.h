#ifndef GRAPHIC_PIPELINE_HEADER
#define GRAPHIC_PIPELINE_HEADER

#include "VkBootstrapHeader.h"
#include "Rasterizer.hpp"
#include "SwapChain.h"
#include "Shader.h"
#include "Viewport.h"
#include "Device.h"

namespace VkBootstrap
{
class GraphicPipeline
{
private:
  VkDevice device;
  VkPipelineLayout pipelineLayout;

public:
  VkPipeline graphicPipeline;

  GraphicPipeline(const Device *device, Shader *shader, SwapChain *swapChain, Viewport *viewport);
  ~GraphicPipeline();
};
}

#endif