#ifndef COMMAND_HEADER
#define COMMAND_HEADER

#include "VkBootstrapHeader.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"

namespace VkBootstrap
{

	class Command
	{
	private:
		VkDevice device = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;
		SwapChain *swapChain = nullptr;
		GraphicPipeline *graphicPipeline = nullptr;

	public:
		std::vector<VkCommandBuffer> commandBuffers;

		Command(const VkDevice &device, SwapChain *swapChain, const VkCommandPool &commandPool, GraphicPipeline *graphicPipeline);

		void begin();
		void end();

		~Command();
	};

}

#endif