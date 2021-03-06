#ifndef RASTERIZER_HEADER
#define RASTERIZER_HEADER

#include "VkBootstrapHeader.h"

namespace VkBootstrap
{
	static VkPipelineRasterizationStateCreateInfo *rasterizers = nullptr;

	class Rasterizer
	{
	private:

	public:
		static inline VkPipelineRasterizationStateCreateInfo *getState()
		{
			if (rasterizers == nullptr) 
			{
				VkPipelineRasterizationStateCreateInfo rasterizer = {};
				rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer.depthClampEnable = VK_FALSE;
				rasterizer.rasterizerDiscardEnable = VK_FALSE;
				rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
				rasterizer.lineWidth = 1.0f;
				//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
				rasterizer.cullMode = VK_CULL_MODE_NONE;
				rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				rasterizer.depthBiasEnable = VK_FALSE;
				rasterizer.depthBiasConstantFactor = 0.0f; // Optional
				rasterizer.depthBiasClamp = 0.0f;          // Optional
				rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

				rasterizers = new VkPipelineRasterizationStateCreateInfo[1];
				rasterizers[0] = rasterizer;
			}

			return rasterizers;
		}
	};
}

#endif