#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkValidationLayerConfiguration.h"
#include "VkPhysicalDeviceManager.h"
#include "VkLogicalDevice.h"

namespace VkDemos
{

class Application
{

private:
  VkDemos::Window *window = nullptr;
  VkInstance vulkanInstance;
  VkDevice *device = nullptr;
  VkSurfaceKHR *surface = nullptr;

  void setupWindow();
  void setupVulkan();
  void setupDebugCallback();

public:
  void run();
  void exit();

  ~Application();
};

} // namespace VkDemos

#endif