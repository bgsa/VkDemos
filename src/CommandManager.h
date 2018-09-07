#ifndef COMMAND_MANAGER_HEADER
#define COMMAND_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"
#include "Device.h"
#include "Command.h"

namespace VkBootstrap
{
	class CommandManager
	{
	private:
		VkDevice device = VK_NULL_HANDLE;
		std::vector<Command*> commands;

		CommandManager(const Device *device);
		
	public:
		VkCommandPool commandPool = VK_NULL_HANDLE;

		static void init(const Device *device);
		Command *createCommand(GraphicPipeline *graphicPipeline, SwapChain *swapChain);

		static CommandManager *getInstance();
		void releaseCommands();

		~CommandManager();
	};
}

#endif