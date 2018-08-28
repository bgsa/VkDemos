#include "File.h"

namespace VkDemos
{

File *File::readFile(const std::string &filename)
{
    File *newFile = new File;

    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file: " + filename);

    size_t fileSize = (size_t)file.tellg();

    newFile->length = fileSize;
    newFile->content = new char[fileSize];

    file.seekg(0);
    file.read(newFile->content, fileSize);
    file.close();

    return newFile;
}

File::~File()
{
    if (content != nullptr)
    {
        delete[] content;
        content = nullptr;
    }
}

} // namespace VkDemos