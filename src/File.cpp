#include "File.h"

namespace VkDemos
{

	inline char getFileSeparator() 
	{
		return FILE_SEPARATOR;
	}

	std::string File::getCurrentDir()
	{
		char currentDir[MAX_DIRECTORY_NAME];

#if WINDOWS
		GetCurrentDirectoryA(MAX_DIRECTORY_NAME, currentDir);
#endif
#if LINUX
		GetCurrentDir(currentDir, sizeof(currentDir));
#endif

		return std::string(currentDir) + FILE_SEPARATOR;
	}

	string File::getAbsolutePath(string relativePath) 
	{
		return string(File::getCurrentDir()) + relativePath;
	}

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