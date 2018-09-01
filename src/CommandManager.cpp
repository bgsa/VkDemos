#include "CommandManager.h"

namespace VkDemos
{

static CommandManager *commandManager = nullptr;

void CommandManager::init(const Device *device, SwapChain *swapChain, GraphicPipeline *graphicPipeline)
{
    commandManager = new CommandManager(device);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = QueueManager::getGraphicQueueFamilyIndex(device->physicalDevice);
    poolInfo.flags = 0; // Optional

    VkResult operationResult = vkCreateCommandPool(device->logicalDevice, &poolInfo, nullptr, &commandManager->commandPool);

    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create command pool!");
}

Command *CommandManager::createCommand(GraphicPipeline *graphicPipeline, SwapChain *swapChain)
{
    Command *command = new Command(device, swapChain, commandPool, graphicPipeline);
    return command;
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