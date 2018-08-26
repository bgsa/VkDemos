#ifndef SET_HELPER_HEADER
#define SET_HELPER_HEADER

#include <vector>
#include <set>
#include <cstring>

namespace VkDemos
{
class SetHelper
{
  public:
    static std::vector<const char *> convertToVector(std::set<std::string> stringSet)
    {
        std::vector<const char *> result;

        for (std::string value : stringSet)
        {
            cout << "VER: " << value.c_str() << endl;
            const char *valueAsCharArray = value.c_str();
            result.push_back(valueAsCharArray);
        }

        return result;
    }

    static const char **convertToCharArray(const set<string> &values)
    {
        const char **array = new const char *[values.size()];
        size_t i = 0;

        for (string value : values)
        {
            array[i] = value.c_str();
            i++;
        }
    }

    template <typename T>
    static void printContent(const set<T> &values)
    {
        for (T value : values)
            VkLogger::getOutputStream() << value << endl;
    }
};
} // namespace VkDemos

#endif