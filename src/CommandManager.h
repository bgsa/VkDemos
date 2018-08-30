#ifndef COMMAND_MANAGER_HEADER
#define COMMAND_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "VkQueueFamily.h"
#include "VkSwapChain.h"
#include "GraphicPipeline.h"

namespace VkDemos
{
class CommandManager
{
  private:
    VkDevice device = VK_NULL_HANDLE;

    CommandManager(const VkDevice &device);

  public:
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    static void init(const VkPhysicalDevice &physicalDevice, const VkDevice &device, VkQueueFamily *queueFamily, VkSwapChain *swapChain, GraphicPipeline *graphicPipeline);
    static CommandManager *getInstance();

    ~CommandManager();
};
} // namespace VkDemos

#endif