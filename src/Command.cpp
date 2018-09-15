#include "Command.h"

namespace VkBootstrap
{

	Command::Command(const VkDevice &device, SwapChain *swapChain, const VkCommandPool &commandPool, GraphicPipeline *graphicPipeline)
	{
		this->device = device;
		this->swapChain = swapChain;
		this->graphicPipeline = graphicPipeline;
		this->commandPool = commandPool;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkResult operationResult = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
	}

	void Command::begin(size_t frameBufferIndex, Viewport* viewport)
	{
		VkRenderPassBeginInfo renderPassInfo = swapChain->getRenderPassBegin(frameBufferIndex);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //if the command is used only one time and reset after rendered
		//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //if the command is used many times after reconrding
		//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; //if the command is secondary and is inside a render pass

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline->graphicPipeline);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline->pipelineLayout, 0, (uint32_t) graphicPipeline->descriptorSets.size(), graphicPipeline->descriptorSets.data(), 0, nullptr);

		vkCmdSetViewport(commandBuffer, 0, 1, viewport->getVkViewport());
		vkCmdSetScissor(commandBuffer, 0, 1, viewport->getVkScissors());
	}

	void Command::end()
	{
		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}

	Command::~Command()
	{
		if (commandBuffer != VK_NULL_HANDLE) 
		{
			vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
			commandBuffer = VK_NULL_HANDLE;
		}
	}

}
