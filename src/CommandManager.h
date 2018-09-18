#ifndef COMMAND_MANAGER_HEADER
#define COMMAND_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"
#include "Device.h"
#include "Command.h"
#include "CopyCommand.h"

namespace VkBootstrap
{
	class CommandManager
	{
	private:
		VkDevice device = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;
		VkCommandPool copyCommandPool = VK_NULL_HANDLE;
		VkQueue transferQueue = VK_NULL_HANDLE;

		std::vector<Command*> commands;
		std::vector<CopyCommand*> copyCommands;

		CommandManager(const Device *device);
		
	public:

		inline size_t getCommandsCount();
		VkCommandBuffer* getCommandBuffers();
		
		static void init(const Device *device);
		Command *createCommand(GraphicPipeline *graphicPipeline, SwapChain *swapChain);
		CopyCommand *createCopyCommand();

		static CommandManager *getInstance();
		void releaseCommands();

		~CommandManager();
	};
}

#endif