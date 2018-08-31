#ifndef COMMAND_MANAGER_HEADER
#define COMMAND_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "QueueManager.h"
#include "VkSwapChain.h"
#include "GraphicPipeline.h"
#include "Device.h"

namespace VkDemos
{
class CommandManager
{
private:
  VkDevice device = VK_NULL_HANDLE;

  CommandManager(const Device *device);

public:
  VkCommandPool commandPool = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> commandBuffers;

  static void init(const Device *device, VkSwapChain *swapChain, GraphicPipeline *graphicPipeline);
  static CommandManager *getInstance();

  ~CommandManager();
};
} // namespace VkDemos

#endif