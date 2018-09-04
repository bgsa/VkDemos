#ifndef FILE_HEADER
#define FILE_HEADER

#include "VkBootstrapHeader.h"
#include <fstream>
#include <stdio.h>

#ifdef WINDOWS
	#define FILE_SEPARATOR '\\'
#endif

#ifdef LINUX
	#include <unistd.h>

	#define GetCurrentDir getcwd

	#define FILE_SEPARATOR '/'
#endif

namespace VkBootstrap
{
class File
{
  private:
	  const static size_t MAX_DIRECTORY_NAME = 256;

  public:
    size_t length = 0;
    char *content = nullptr;

	static inline char getFileSeparator();
	static std::string getCurrentDir();
	static std::string getAbsolutePath(std::string relativePath);
    static File *readFile(const std::string &filename);

    ~File();
};
}

#endif