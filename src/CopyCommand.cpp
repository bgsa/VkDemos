#include "CopyCommand.h"

namespace VkBootstrap
{

	CopyCommand::CopyCommand(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue& transferQueue)
	{
		this->device = device;
		this->commandPool = commandPool;
		this->transferQueue = transferQueue;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;
		
		VkResult operationResult = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
	}

	void CopyCommand::begin(VkDeviceSize bufferSize, VkDeviceSize sourceOffset, VkDeviceSize destinationOffset)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = sourceOffset;
		copyRegion.dstOffset = destinationOffset;
		copyRegion.size = bufferSize;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	void CopyCommand::execute(VkBuffer sourceBuffer, VkBuffer destinationBuffer, uint32_t copyRegionCount, VkBufferCopy* copyRegion) 
	{
		vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, copyRegionCount, copyRegion);
	}

	void CopyCommand::end()
	{
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
		
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);

		vkQueueWaitIdle(transferQueue);
	}

	CopyCommand::~CopyCommand()
	{
		if (commandBuffer != VK_NULL_HANDLE) 
		{
			vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
			commandBuffer = VK_NULL_HANDLE;
		}
	}

}
