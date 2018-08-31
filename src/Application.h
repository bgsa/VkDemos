#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkValidationLayerConfiguration.h"
#include "VkPhysicalDeviceManager.h"
#include "Device.h"
#include "VkSwapChain.h"
#include "Shader.h"
#include "Viewport.h"
#include "Rasterizer.hpp"
#include "WindowInputDevice.h"
#include "GraphicPipeline.h"
#include "CommandManager.h"

namespace VkDemos
{

class Application : public WindowInputDeviceHandler
{

private:
  VkDemos::Window *window = nullptr;
  VkInstance vulkanInstance = VK_NULL_HANDLE;
  Device *device = nullptr;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkSwapChain *swapChain = nullptr;
  GraphicPipeline *graphicPipeline = nullptr;
  CommandManager *commandManager = nullptr;

  VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
  VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;

  bool isRunning = true;

  void setupWindow();
  void setupVulkan();
  void setupSurface();
  void setupDevice();
  void setupSemaphores();
  void setupDebugCallback();

  void window_OnClose();
  void window_OnResize(int width, int height);

public:
  void run();
  void exit();

  ~Application();
};

} // namespace VkDemos

#endif