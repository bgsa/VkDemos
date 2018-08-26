#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkValidationLayerConfiguration.h"
#include "VkPhysicalDeviceManager.h"
#include "VkLogicalDevice.h"
#include "VkSwapChain.h"

namespace VkDemos
{

class Application
{

private:
  VkDemos::Window *window = nullptr;
  VkInstance vulkanInstance = VK_NULL_HANDLE;
  VkDevice *device = nullptr;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkQueueFamily *queueFamily = nullptr;
  VkSwapChain *swapChain = nullptr;

  void setupWindow();
  void setupVulkan();
  void setupSurface();
  void setupDebugCallback();

public:
  void run();
  void exit();

  ~Application();
};

} // namespace VkDemos

#endif