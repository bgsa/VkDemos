#include "Window.h"

using namespace std;

int main(int argc, char const *argv[])
{
    VkDemos::Window *window = new VkDemos::Window;

    window->init();

    delete window;

    return 0;
}
