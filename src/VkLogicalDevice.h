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
  static VkDevice *createLogicalDevice(const VkInstance &vulkanInstance, VkSurfaceKHR *surface, vector<string> extensions);
  static VkDevice *createLogicalDevice(const VkPhysicalDevice &physicalDevice, VkSurfaceKHR *surface, vector<string> extensions);
};
} // namespace VkDemos

#endif