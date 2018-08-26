#ifndef VK_LOGGER_HEADER
#define VK_LOGGER_HEADER

#include <iostream>

using namespace std;

namespace VkDemos
{
class VkLogger
{
  public:
    static void error(const exception &e)
    {
        error(e.what());
    }

    static void error(int code, const char *message)
    {
        cerr << "Error " << code << ": " << message << endl;
    }

    static void error(const char *message)
    {
        cerr << "Error: " << message << endl;
    }

    static void info(const char *message)
    {
        cout << "Info: " << message << endl;
    }

    static void info(const string message)
    {
        cout << "Info: " << message << endl;
    }

    static void debug(const char *message)
    {
        cout << "Debug: " << message << endl;
    }

    static ostream &getOutputStream()
    {
        return cout;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        string infoLevel = "Info: ";

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            infoLevel = "Warning: ";
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            infoLevel = "Error: ";

        cout
            << infoLevel << "Instance layer: " << endl
            << "    Message Type: " << messageType << endl
            << "    Message Id: " << pCallbackData->pMessageIdName << endl
            << "    Massage: " << pCallbackData->pMessage << endl
            << "    Callback Type: " << pCallbackData->sType << endl;

        return VK_FALSE;
    }
};
} // namespace VkDemos

#endif