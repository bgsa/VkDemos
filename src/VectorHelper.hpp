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
};
} // namespace VkDemos

#endif