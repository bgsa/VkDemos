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

    static void debug(const char *message)
    {
        cout << "Debug: " << message << endl;
    }
};
} // namespace VkDemos

#endif