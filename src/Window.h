#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include "VkBootstrapHeader.h"
#include "WindowInfo.h"
#include "WindowInputDevice.h"
#include "Size.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

namespace VkBootstrap
{

class Window : public WindowInputDevice
{
private:
  GLFWwindow *windowHandler;

public:
  void setup(VkBootstrap::WindowInfo &windowInfo);
  void update(long long elapsedTime);

  Size getSize();

  std::vector<const char *> getRequiredExtensions();
  void printRequiredExtensions();

  void createSurface(const VkInstance &vulkanInstance, VkSurfaceKHR *surface);

  ~Window();
};

}

#endif