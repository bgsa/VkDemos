#include "RendererObject.h"

namespace VkBootstrap
{
		
	RendererObject::RendererObject(Device* device, SwapChain* swapChain, Viewport* viewport, VkDescriptorSetLayout* descriptorSetLayout, const std::vector<VkDescriptorSet>& descriptorSets)
	{
		this->device = device;
		this->viewport = viewport;

		shader = Shader::createShader(device, "resources/shaders/vert.spv", "resources/shaders/frag.spv");
		
		size_t vertexBufferSize = sizeof(vertices[0]) * vertices.size();
		size_t indexBufferSize = sizeof(indices[0]) * indices.size();
		size_t bufferSize = vertexBufferSize + indexBufferSize;

		char *bufferData = (char*) malloc(bufferSize);
		std::memcpy(bufferData, vertices.data(), vertexBufferSize);
		std::memcpy(&bufferData[vertexBufferSize], indices.data(), indexBufferSize);

		dataBuffer = new MemoryBuffer(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bufferSize, bufferData);

		delete[] bufferData;

		createPipeline(device, swapChain, descriptorSetLayout, descriptorSets);
	}

	void RendererObject::update(long long elapsedTime) 
	{
	}

	void RendererObject::createPipeline(Device* device, SwapChain* swapChain, VkDescriptorSetLayout* descriptorSetLayout, const std::vector<VkDescriptorSet>& descriptorSets)
	{
		//create vertex input ...
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

		graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport, &vertexInputInfo, descriptorSetLayout, descriptorSets);
	}

	void RendererObject::render(SwapChain* swapChain, uint32_t imageIndex)
	{
		VkBuffer vertexBuffers[] = { dataBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		size_t vertexBufferSize = sizeof(vertices[0]) * vertices.size();

		CommandManager* commandManager = CommandManager::getInstance();
		Command *command = commandManager->createCommand(graphicPipeline, swapChain);
		command->begin(imageIndex, viewport);

		vkCmdBindVertexBuffers(command->commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(command->commandBuffer, dataBuffer->getBuffer(), vertexBufferSize, VK_INDEX_TYPE_UINT16);

		vkCmdDrawIndexed(command->commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		command->end();
	}

	RendererObject::~RendererObject() 
	{
		if (shader != nullptr)
		{
			delete shader;
			shader = nullptr;
		}

		if (dataBuffer != nullptr) 
		{
			delete dataBuffer;
			dataBuffer = nullptr;
		}

		if (graphicPipeline != nullptr)
		{
			delete graphicPipeline;
			graphicPipeline = nullptr;
		}
	}
	
}