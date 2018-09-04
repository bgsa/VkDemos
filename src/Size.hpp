#ifndef SIZE_HEADER
#define SIZE_HEADER

namespace VkBootstrap
{
class Size
{
  public:
    int width = 0;
    int height = 0;

    Size(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
};
}

#endif