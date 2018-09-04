#ifndef VK_INSTANCE_LAYER_CONFIGURATION_HEADER
#define VK_INSTANCE_LAYER_CONFIGURATION_HEADER

#include "VkBootstrapHeader.h"

namespace VkBootstrap
{
class VkInstanceLayerConfiguration
{
private:
public:
  static uint32_t getInstanceLayersSupportedCount();
  static std::vector<VkLayerProperties> getInstanceLayersSupported();
  static void printInstanceLayersSupported();
  static bool isInstanceLayerSupported(const std::string &validationLayerName);
  static void destroyDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks *pAllocator);
  static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                               const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                               const VkAllocationCallbacks *pAllocator);
};
}

#endif