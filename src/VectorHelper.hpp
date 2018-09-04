#ifndef VECTOR_HELPER_HEADER
#define VECTOR_HELPER_HEADER

#include <vector>
#include <cstring>

namespace VkBootstrap
{
class VectorHelper
{
  public:
    static char **convertToCharArray(const std::vector<std::string> &vector)
    {
        char **array = new char *[vector.size()];

        for (size_t i = 0; i != vector.size(); i++)
            array[i] = const_cast<char *>(vector[i].c_str());

        return array;
    }

    static std::set<std::string> convertToSetString(const std::vector<const char *> &vector)
    {
		std::set<std::string> result;

        for (size_t i = 0; i != vector.size(); i++)
            result.insert(std::string(vector[i]));

        return result;
    }

    static void convertToCharArrayVector(const std::vector<std::string> &vector, std::vector<const char *> &newVector)
    {
        for (std::string value : vector)
            newVector.push_back(value.c_str());
    }

    template <typename T>
    static void printContent(const std::vector<T> &vector)
    {
        for (T value : vector)
            VkLogger::getOutputStream() << value << std::endl;
    }
};
}

#endif