#ifndef SET_HELPER_HEADER
#define SET_HELPER_HEADER

#include <vector>
#include <set>
#include <cstring>

namespace VkBootstrap
{
class SetHelper
{
  public:
    static std::vector<const char *> convertToVector(std::set<std::string> stringSet)
    {
        std::vector<const char *> result;

        for (std::string value : stringSet)
        {
			std::cout << "VER: " << value.c_str() << std::endl;
            const char *valueAsCharArray = value.c_str();
            result.push_back(valueAsCharArray);
        }

        return result;
    }

    static const char **convertToCharArray(const std::set<std::string> &values)
    {
        const char **array = new const char *[values.size()];
        size_t i = 0;

        for (std::string value : values)
        {
            array[i] = value.c_str();
            i++;
        }

        return array;
    }

    template <typename T>
    static void printContent(const std::set<T> &values)
    {
        for (T value : values)
            VkLogger::getOutputStream() << value << std::endl;
    }
};
}

#endif