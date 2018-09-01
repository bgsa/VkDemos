#ifndef COMMAND_MANAGER_HEADER
#define COMMAND_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "QueueManager.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"
#include "Device.h"
#include "Command.h"

namespace VkDemos
{
class CommandManager
{
private:
  VkDevice device = VK_NULL_HANDLE;

  CommandManager(const Device *device);

public:
  VkCommandPool commandPool = VK_NULL_HANDLE;

  static void init(const Device *device, SwapChain *swapChain, GraphicPipeline *graphicPipeline);
  Command *createCommand(GraphicPipeline *graphicPipeline, SwapChain *swapChain);

  static CommandManager *getInstance();

  ~CommandManager();
};
} // namespace VkDemos

#endif