#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkValidationLayerConfiguration.h"

namespace VkDemos
{

class Application
{

private:
  VkDemos::Window *window;
  VkInstance vulkanInstance;

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