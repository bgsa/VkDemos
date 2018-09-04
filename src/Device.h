#ifndef DEVICE_HEADER
#define DEVICE_HEADER

#include "VkBootstrapHeader.h"
#include "VkPhysicalDeviceManager.h"
#include "VkInstanceLayerConfiguration.h"

namespace VkBootstrap
{
class Device
{
private:
  void createDevices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);
  void createQueues(const VkSurfaceKHR &surface);

public:
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice logicalDevice = VK_NULL_HANDLE;
  Queue *graphicsQueue = nullptr;
  Queue *presentQueue = nullptr;

  Device(const VkInstance &vulkanInstance, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);
  Device(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);

  ~Device();
};
}

#endif