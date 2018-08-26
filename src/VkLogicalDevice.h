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
  static VkDevice *createLogicalDevice(const VkInstance &vulkanInstance, VkSurfaceKHR *surface);
  static VkDevice *createLogicalDevice(const VkPhysicalDevice &physicalDevice, VkSurfaceKHR *surface);
};
} // namespace VkDemos

#endif