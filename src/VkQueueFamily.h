#ifndef VK_QUEUE_FAMILY_HEADER
#define VK_QUEUE_FAMILY_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class VkQueueFamily
{
public:
  VkQueue graphicsQueue;
  uint32_t graphicsQueueIndex = -1;

  VkQueue presentQueue;
  uint32_t presentQueueIndex = -1;

  static vector<VkQueueFamilyProperties> getQueueFamilies(const VkPhysicalDevice &device);
  static bool hasGraphicQueueFamily(vector<VkQueueFamilyProperties> queueFamilies);
  static bool hasGraphicQueueFamily(const VkPhysicalDevice &device);
  static bool isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily);
  static uint32_t getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice);
  static uint32_t getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);
  static VkQueueFamily *createQueueFamily(const VkPhysicalDevice &physicalDevice, const VkDevice &device, const VkSurfaceKHR &surface);
};
} // namespace VkDemos

#endif