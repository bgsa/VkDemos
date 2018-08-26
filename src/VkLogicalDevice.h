#ifndef VK_LOGICAL_DEVICE_HEADER
#define VK_LOGICAL_DEVICE_HEADER

#include "VkDemosHeader.h"
#include "VkPhysicalDeviceManager.h"
#include "VkValidationLayerConfiguration.h"

namespace VkDemos
{
class VkLogicalDevice
{
public:
  static VkDevice *createLogicalDevice(const VkInstance &vulkanInstance, const VkSurfaceKHR &surface, const vector<const char *> &extensions);
  static VkDevice *createLogicalDevice(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const vector<const char *> &extensions);
};
} // namespace VkDemos

#endif