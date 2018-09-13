#include "Viewport.h"

namespace VkBootstrap
{

Viewport::Viewport(int width, int height)
{
	setSize(width, height);
}

void Viewport::setSize(int width, int height)
{
	this->width = width;
	this->height = height;

	if (viewport != nullptr) 
	{
		viewport->width = (float)width;
		viewport->height = (float)height;
	}

	if (scissor != nullptr) 
	{
		scissor->extent = VkExtent2D{ (uint32_t)width, (uint32_t)height };
	}
}

VkViewport* Viewport::getVkViewport() 
{
	return viewport;
}

VkRect2D* Viewport::getVkScissors() 
{
	return scissor;
}

VkPipelineViewportStateCreateInfo *Viewport::getViewportState()
{
	if (viewport == nullptr)
		viewport = new VkViewport;

    viewport->x = 0.0f;
    viewport->y = 0.0f;
    viewport->width = (float)width;
    viewport->height = (float)height;
    viewport->minDepth = 0.0f;
    viewport->maxDepth = 1.0f;

	if (scissor == nullptr)
		scissor = new VkRect2D;

    scissor->offset = {0, 0};
    scissor->extent = VkExtent2D{(uint32_t)width, (uint32_t)height};

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = scissor;

    VkPipelineViewportStateCreateInfo *viewportStates = new VkPipelineViewportStateCreateInfo[1];
    viewportStates[0] = viewportState;

    return viewportStates;
}

}
