#ifndef VK_VALIDATION_LAYER_CONFIGURATION_HEADER
#define VK_VALIDATION_LAYER_CONFIGURATION_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class VkValidationLayerConfiguration
{
private:

public:
  static uint32_t getValidationLayersSupportedCount();
  static vector<VkLayerProperties> getValidationLayersSupported();
  static void printValidationLayersSupported();
  static bool isValidationLayerSupported(const string &validationLayerName);
  static void destroyDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks *pAllocator);
  static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                               const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                               const VkAllocationCallbacks *pAllocator);
};
} // namespace VkDemos

#endif