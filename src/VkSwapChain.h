#ifndef VK_SWAP_CHAIN_HEADER
#define VK_SWAP_CHAIN_HEADER

#include "VkDemosHeader.h"
#include "VkQueueFamily.h"
#include "VkSwapChainProperties.hpp"

namespace VkDemos
{
class VkSwapChain
{
private:
  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &surfaceFormats);
  static VkPresentModeKHR chooseSwapPresentMode(const vector<VkPresentModeKHR> &presentModes);
  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfacecapabilities);

public:
  VkSurfaceFormatKHR surfaceFormat;
  VkPresentModeKHR presentMode;
  VkExtent2D extent;
  VkSwapchainKHR vulkanSwapChain;

  static VkSwapChain *createSwapChain(const VkPhysicalDevice &physicalDevice, const VkDevice &logicalDevice, const VkSurfaceKHR &surface, const VkQueueFamily &queueFamily);
};
} // namespace VkDemos

#endif