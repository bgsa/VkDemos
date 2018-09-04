
#include "Application.h"

int main(int argc, char const *argv[])
{
    VkBootstrap::Application *application = new VkBootstrap::Application;

    try
    {
        application->run();
    }
    catch (const std::exception &e)
    {
		VkBootstrap::VkLogger::error(e);

        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}