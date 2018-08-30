#ifndef RASTERIZER_HEADER
#define RASTERIZER_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class Rasterizer
{
  public:
    static VkPipelineRasterizationStateCreateInfo *getState()
    {
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f;          // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

        VkPipelineRasterizationStateCreateInfo *rasterizers = new VkPipelineRasterizationStateCreateInfo[1];
        rasterizers[0] = rasterizer;

        return rasterizers;
    }
};
} // namespace VkDemos

#endif