#ifndef FILE_HEADER
#define FILE_HEADER

#include <cstring>
#include <vector>
#include <fstream>

namespace VkDemos
{
class File
{
  private:
  public:
    size_t length = 0;
    char *content = nullptr;

    static File *readFile(const std::string &filename);

    ~File();
};
} // namespace VkDemos

#endif