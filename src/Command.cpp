#include "Command.h"

namespace VkBootstrap
{

	Command::Command(const VkDevice &device, SwapChain *swapChain, const VkCommandPool &commandPool, GraphicPipeline *graphicPipeline)
	{
		this->device = device;
		this->swapChain = swapChain;
		this->graphicPipeline = graphicPipeline;
		this->commandPool = commandPool;

		uint32_t commandBufferCount = 1;
		commandBuffers.resize(commandBufferCount);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = commandBufferCount;

		VkResult operationResult = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
	}

	void Command::begin(size_t frameBufferIndex)
	{
		for (size_t i = 0; i != commandBuffers.size(); i++)
		{
			VkCommandBuffer commandBuffer = commandBuffers[i];
			VkRenderPassBeginInfo renderPassInfo = swapChain->getRenderPassBegin(frameBufferIndex);			

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pInheritanceInfo = nullptr;
			//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //if the command is used many times after reconrding
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //if the command is used only one time and reset after rendered
			//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; //if the command is secondary and is inside a render pass

			if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
				throw std::runtime_error("failed to begin recording command buffer!");

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline->graphicPipeline);
		}
	}

	void Command::end()
	{
		for (VkCommandBuffer commandBuffer : commandBuffers)
		{
			vkCmdEndRenderPass(commandBuffer);

			if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
				throw std::runtime_error("failed to record command buffer!");
		}
	}

	Command::~Command()
	{
		if (commandBuffers.size() != 0)
			vkFreeCommandBuffers(device, commandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
	}

}
