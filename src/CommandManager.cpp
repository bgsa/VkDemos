#include "CommandManager.h"

namespace VkBootstrap
{
	static CommandManager *commandManager = nullptr;

	size_t CommandManager::getCommandsCount() 
	{
		return commands.size();
	}

	VkCommandBuffer* CommandManager::getCommandBuffers()
	{	
		VkCommandBuffer* commandsTemp = new VkCommandBuffer[getCommandsCount()];

		for (size_t i = 0; i < getCommandsCount(); i++)
			commandsTemp[i] = commands[i]->commandBuffer;

		return commandsTemp;
	}

	void CommandManager::init(const Device *device)
	{
		commandManager = new CommandManager(device);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = device->queueManager->getGraphicQueueFamily()->getIndex();
		//poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //commands can be reused. Reset is explicit
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		VkResult operationResult = vkCreateCommandPool(device->logicalDevice, &poolInfo, nullptr, &commandManager->commandPool);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to create command pool!");
	}

	Command *CommandManager::createCommand(GraphicPipeline *graphicPipeline, SwapChain *swapChain)
	{
		Command *command = new Command(device, swapChain, commandPool, graphicPipeline);

		commands.push_back(command);

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

	void CommandManager::releaseCommands() 
	{
		for (Command* command : commands)
			delete command;

		commands.clear();
	}

	CommandManager::~CommandManager()
	{
		releaseCommands();

		vkDestroyCommandPool(device, commandPool, nullptr);
		commandPool = VK_NULL_HANDLE;
	}

}
