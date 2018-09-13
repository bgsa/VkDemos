#include "RendererObject.h"

namespace VkBootstrap
{
	
	RendererObject::RendererObject(Device* device, SwapChain* swapChain, Viewport* viewport) 
	{
		shader = Shader::createShader(device, "resources/shaders/vert.spv", "resources/shaders/frag.spv");

		memoryBuffer = new MemoryBuffer(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(vertices[0]) * vertices.size(), (void*)vertices.data());

		createPipeline(device, swapChain, viewport);
	}

	void RendererObject::createPipeline(Device* device, SwapChain* swapChain, Viewport* viewport) 
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = { };
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport, &vertexInputInfo);
	}

	void RendererObject::render(CommandManager* commandManager, SwapChain* swapChain, uint32_t imageIndex, Viewport* viewport)
	{
		VkBuffer vertexBuffers[] = { memoryBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };

		Command *command = commandManager->createCommand(graphicPipeline, swapChain);
		command->begin(imageIndex, viewport);

		vkCmdBindVertexBuffers(command->commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdDraw(command->commandBuffer, (uint32_t)vertices.size(), 1, 0, 0);

		command->end();
	}

	RendererObject::~RendererObject() 
	{
		if (shader != nullptr)
		{
			delete shader;
			shader = nullptr;
		}

		if (memoryBuffer != nullptr) 
		{
			delete memoryBuffer;
			memoryBuffer = nullptr;
		}

		if (graphicPipeline != nullptr)
		{
			delete graphicPipeline;
			graphicPipeline = nullptr;
		}
	}
	
}