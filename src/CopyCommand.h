#ifndef COPY_COMMAND_HEADER
#define COPY_COMMAND_HEADER

#include "VkBootstrapHeader.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"

namespace VkBootstrap
{

	class CopyCommand
	{
	private:
		VkDevice device = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;
		VkQueue transferQueue = VK_NULL_HANDLE;

	public:
		VkCommandBuffer commandBuffer;

		CopyCommand(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue& transferQueue);

		void begin(VkDeviceSize bufferSize, VkDeviceSize sourceOffset = 0, VkDeviceSize destinationOffset = 0);
		void execute(VkBuffer sourceBuffer, VkBuffer destinationBuffer, uint32_t copyRegionCount, VkBufferCopy* copyRegion);
		void end();

		~CopyCommand();
	};

}

#endif