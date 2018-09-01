
#include "Application.h"

using namespace std;

int main(int argc, char const *argv[])
{
    VkDemos::Application *application = new VkDemos::Application;

    try
    {
        application->run();
    }
    catch (const exception &e)
    {
        VkDemos::VkLogger::error(e);

        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}