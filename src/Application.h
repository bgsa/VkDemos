#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"
#include "VkExtensionsConfiguration.h"

namespace VkDemos
{

class Application
{

  private:
    VkDemos::Window *window;

  public:
    void run();
    void exit();

    ~Application();
};

} // namespace VkDemos

#endif