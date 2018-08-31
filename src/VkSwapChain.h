#ifndef VK_SWAP_CHAIN_HEADER
#define VK_SWAP_CHAIN_HEADER

#include "VkDemosHeader.h"
#include "Device.h"
#include "VkSwapChainProperties.hpp"

namespace VkDemos
{
class VkSwapChain
{
private:
  VkDevice device;

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &surfaceFormats);
  static VkPresentModeKHR chooseSwapPresentMode(const vector<VkPresentModeKHR> &presentModes);
  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfacecapabilities);

  void createImageViews(const VkDevice &device);
  void createRenderPass(const VkDevice &device);
  void createFramebuffers(const VkDevice &device);

public:
  VkSurfaceFormatKHR surfaceFormat;
  VkPresentModeKHR presentMode;
  VkExtent2D extent;
  VkSwapchainKHR vulkanSwapChain = VK_NULL_HANDLE;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> imageViews;
  VkRenderPass renderPass;
  std::vector<VkFramebuffer> framebuffers;

  static VkSwapChain *createSwapChain(const Device *device, const VkSurfaceKHR &surface);

  ~VkSwapChain();
};
} // namespace VkDemos

#endif