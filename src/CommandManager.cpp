#include "CommandManager.h"

namespace VkDemos
{

static CommandManager *commandManager = nullptr;

void CommandManager::init(const Device *device, VkSwapChain *swapChain, GraphicPipeline *graphicPipeline)
{
    commandManager = new CommandManager(device);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = QueueManager::getGraphicQueueFamilyIndex(device->physicalDevice);
    poolInfo.flags = 0; // Optional

    VkResult operationResult = vkCreateCommandPool(device->logicalDevice, &poolInfo, nullptr, &commandManager->commandPool);

    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create command pool!");

    //create command buffer
    commandManager->commandBuffers.resize(swapChain->framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandManager->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandManager->commandBuffers.size();

    operationResult = vkAllocateCommandBuffers(device->logicalDevice, &allocInfo, commandManager->commandBuffers.data());

    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to allocate command buffers!");

    for (size_t i = 0; i < commandManager->commandBuffers.size(); i++)
    {
        VkCommandBuffer commandBuffer = commandManager->commandBuffers[i];

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");

        //start rendering pass
        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->renderPass;
        renderPassInfo.framebuffer = swapChain->framebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline->graphicPipeline);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");
    }
}

CommandManager::CommandManager(const Device *device)
{
    this->device = device->logicalDevice;
}

CommandManager *CommandManager::getInstance()
{
    if (commandManager != nullptr)
        return commandManager;

    return commandManager;
}

CommandManager::~CommandManager()
{
    vkDestroyCommandPool(device, commandPool, nullptr);
    commandPool = VK_NULL_HANDLE;
}

} // namespace VkDemos