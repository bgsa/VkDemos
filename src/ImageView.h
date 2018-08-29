#ifndef IMAGE_VIEW_HEADER
#define IMAGE_VIEW_HEADER

#include "VkDemosHeader.h"
#include "VkSwapChain.h"

namespace VkDemos
{
class ImageView
{
private:
  VkDevice *device = nullptr;

public:
  std::vector<VkImageView> views;
  static ImageView *createImageViews(VkDevice *device, VkSwapChain *swapChain);

  ~ImageView();
};
} // namespace VkDemos

#endif