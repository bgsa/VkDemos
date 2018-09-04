#ifndef VK_LOGGER_HEADER
#define VK_LOGGER_HEADER

#include <iostream>

namespace VkBootstrap
{
class VkLogger
{
  public:
    static void error(const std::exception &e)
    {
		error(e.what());
    }

    static void error(int code, const char *message)
    {
		std::cerr << "Error " << code << ": " << message << std::endl;
    }

    static void error(const char *message)
    {
		std::cerr << "Error: " << message << std::endl;
    }

    static void info(const char *message)
    {
		std::cout << "Info: " << message << std::endl;
    }

    static void info(const std::string message)
    {
		std::cout << "Info: " << message << std::endl;
    }

    static void debug(const char *message)
    {
		std::cout << "Debug: " << message << std::endl;
    }

    static std::ostream &getOutputStream()
    {
        return std::cout;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        std::string infoLevel = "Info: ";

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            infoLevel = "Warning: ";
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            infoLevel = "Error: ";

		std::cout
            << infoLevel << "Instance layer: " << std::endl
            << "    Message Type: " << messageType << std::endl
            << "    Message Id: " << pCallbackData->pMessageIdName << std::endl
            << "    Massage: " << pCallbackData->pMessage << std::endl
            << "    Callback Type: " << pCallbackData->sType << std::endl;

        return VK_FALSE;
    }
};
}

#endif