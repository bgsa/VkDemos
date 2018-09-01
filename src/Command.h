#ifndef COMMAND_HEADER
#define COMMAND_HEADER

#include "VkDemosHeader.h"
#include "SwapChain.h"
#include "GraphicPipeline.h"

namespace VkDemos
{

class Command
{
  private:
    SwapChain *swapChain = nullptr;
    GraphicPipeline *graphicPipeline = nullptr;

  public:
    std::vector<VkCommandBuffer> commandBuffers;

    Command(const VkDevice &device, SwapChain *swapChain, const VkCommandPool &commandPool, GraphicPipeline *graphicPipeline);

    void begin();
    void end();

    ~Command();
};

} // namespace VkDemos

#endif