#ifndef VECTOR_HELPER_HEADER
#define VECTOR_HELPER_HEADER

#include <vector>
#include <cstring>

using namespace std;

namespace VkDemos
{
class VectorHelper
{
  public:
    static char **convertToCharArray(const vector<string> &vector)
    {
        char **array = new char *[vector.size()];

        for (size_t i = 0; i != vector.size(); i++)
            array[i] = const_cast<char *>(vector[i].c_str());

        return array;
    }

    static set<string> convertToSetString(const vector<const char *> &vector)
    {
        set<string> result;

        for (size_t i = 0; i != vector.size(); i++)
            result.insert(string(vector[i]));

        return result;
    }

    template <typename T>
    static void printContent(const vector<T> &vector)
    {
        for (T value : vector)
            VkLogger::getOutputStream() << value << endl;
    }
};
} // namespace VkDemos

#endif