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
		VkDevice device = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipelineViewportStateCreateInfo* viewportState = nullptr;

	public:
		VkPipeline graphicPipeline = VK_NULL_HANDLE;

		GraphicPipeline(const Device *device, Shader *shader, SwapChain *swapChain, Viewport *viewport, VkPipelineVertexInputStateCreateInfo* vertexInput);
		~GraphicPipeline();
	};
}

#endif