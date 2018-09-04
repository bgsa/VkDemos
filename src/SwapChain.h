#ifndef SWAP_CHAIN_HEADER
#define SWAP_CHAIN_HEADER

#include "VkBootstrapHeader.h"
#include "Device.h"
#include "SwapChainProperties.hpp"
#include "Window.h"

namespace VkBootstrap
{
class SwapChain
{
private:
  VkDevice device;
  VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
  VkRenderPassBeginInfo renderPassInfo = {};

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats);
  static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes);
  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfacecapabilities, Window *window);

  void createImageViews(const VkDevice &device);
  void createRenderPass(const VkDevice &device);
  void createFramebuffers(const VkDevice &device);

public:
  VkSwapchainKHR vulkanSwapChain = VK_NULL_HANDLE;
  VkSurfaceFormatKHR surfaceFormat;
  VkPresentModeKHR presentMode;
  VkExtent2D extent;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> imageViews;
  VkRenderPass renderPass;
  std::vector<VkFramebuffer> framebuffers;

  static SwapChain *createSwapChain(const Device *device, const VkSurfaceKHR &surface, Window *window);
  VkRenderPassBeginInfo getRenderPassBegin(size_t framebufferIndex);

  ~SwapChain();
};
}

#endif