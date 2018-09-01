#include "Command.h"

namespace VkDemos
{

Command::Command(const VkDevice &device, SwapChain *swapChain, const VkCommandPool &commandPool, GraphicPipeline *graphicPipeline)
{
    this->swapChain = swapChain;
    this->graphicPipeline = graphicPipeline;

    uint32_t commandBufferCount = swapChain->framebuffers.size();
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

void Command::begin()
{
    for (size_t i = 0; i != commandBuffers.size(); i++)
    {
        VkCommandBuffer commandBuffer = commandBuffers[i];
        VkRenderPassBeginInfo renderPassInfo = swapChain->getRenderPassBegin(i);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

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

    commandBuffers.clear();
}

Command::~Command()
{
    end();
}

} // namespace VkDemos