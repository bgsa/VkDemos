#include "Application.h"

namespace VkDemos
{

void Application::run()
{
    VkExtensionsConfiguration::printSupportedExtensions();

    VkDemos::WindowInfo windowInfo("VkDemo", 800, 600);

    window = new VkDemos::Window;
    window->setup(windowInfo);
}

void Application::exit()
{
    if (window != nullptr)
        delete window;
}

Application::~Application()
{
    exit();
}

} // namespace VkDemos