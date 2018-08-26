#ifndef VK_HELPER_HEADER
#define VK_HELPER_HEADER

#include <cstring>
#include <vulkan/vulkan.h>

namespace VkDemos
{
class VkHelper
{
  public:
    static std::string getVkResultDescription(VkResult result)
    {
        switch (result)
        {
        case VK_SUCCESS:
            return "success";
        case VK_NOT_READY:
            return "not ready";
        case VK_TIMEOUT:
            return "timeout";
        case VK_EVENT_SET:
            return "event set";
        case VK_EVENT_RESET:
            return "event reset";
        case VK_INCOMPLETE: //e VK_RESULT_END_RANGE:
            return "incomplete or result end range";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "out of host memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "out of device memory";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "initialization failed";
        case VK_ERROR_DEVICE_LOST:
            return "device lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "memory map failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "layer nor present";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "extension not present";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "feature nor present";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "incompatible driver";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "too many objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "format not supported";
        case VK_ERROR_FRAGMENTED_POOL: //case VK_RESULT_BEGIN_RANGE:
            return "fragmented pool or result begin range";
        case VK_ERROR_OUT_OF_POOL_MEMORY: //case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
            return "out of pool memory";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: //case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
            return "invalid external handle";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "surface lost";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "native window in use";
        case VK_SUBOPTIMAL_KHR:
            return "suboptimal";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "out of date";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "incompatible display";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "validation failed";
        case VK_ERROR_INVALID_SHADER_NV:
            return "invalid shader";
        case VK_ERROR_FRAGMENTATION_EXT:
            return "fragmentation";
        case VK_ERROR_NOT_PERMITTED_EXT:
            return "not permitted";
        case VK_RESULT_MAX_ENUM:
            return "max enum";
        case VK_RESULT_RANGE_SIZE:
            return "range size";

        default:
            return "unknown";
        }
    }
};

} // namespace VkDemos

#endif