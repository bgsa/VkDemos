#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include "VkDemosHeader.h"
#include "WindowInfo.h"
#include "WindowInputDevice.h"
#include "Size.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

namespace VkDemos
{

class Window : public WindowInputDevice
{
private:
  GLFWwindow *windowHandler;

public:
  void setup(VkDemos::WindowInfo &windowInfo);
  void update(long long elapsedTime);

  Size getSize();

  vector<const char *> getRequiredExtensions();
  void printRequiredExtensions();

  void createSurface(const VkInstance &vulkanInstance, VkSurfaceKHR *surface);

  ~Window();
};

} // namespace VkDemos

#endif