#ifndef QUEUE_MANAGER_HEADER
#define QUEUE_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "Queue.hpp"

namespace VkDemos
{
class QueueManager
{
public:
  static vector<VkQueueFamilyProperties> getQueueFamilies(const VkPhysicalDevice &device);
  static bool hasGraphicQueueFamily(vector<VkQueueFamilyProperties> queueFamilies);
  static bool hasGraphicQueueFamily(const VkPhysicalDevice &device);
  static bool isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily);
  static uint32_t getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice);
  static uint32_t getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);
};
} // namespace VkDemos

#endif