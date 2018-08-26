#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include "VkDemosHeader.h"
#include "WindowInfo.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

namespace VkDemos
{

class Window
{
private:
  GLFWwindow *windowHandler;

public:
  void setup(VkDemos::WindowInfo &windowInfo);
  void update();
  vector<string> getRequiredExtensions();
  VkSurfaceKHR *createSurface(const VkInstance &vulkanInstance);
  void printRequiredExtensions();
  ~Window();
};

} // namespace VkDemos

#endif