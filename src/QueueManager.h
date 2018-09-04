#ifndef QUEUE_MANAGER_HEADER
#define QUEUE_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "Queue.hpp"

namespace VkBootstrap
{
class QueueManager
{
public:
  static std::vector<VkQueueFamilyProperties> getQueueFamilies(const VkPhysicalDevice &device);
  static bool hasGraphicQueueFamily(std::vector<VkQueueFamilyProperties> queueFamilies);
  static bool hasGraphicQueueFamily(const VkPhysicalDevice &device);
  static bool isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily);
  static uint32_t getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice);
  static uint32_t getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);
};
}

#endif