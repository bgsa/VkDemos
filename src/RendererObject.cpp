#include "RendererObject.h"

namespace VkBootstrap
{
		
	RendererObject::RendererObject(Device* device, SwapChain* swapChain, Viewport* viewport) 
	{
		this->device = device;
		this->viewport = viewport;

		float aspectRatio = viewport->getAspectRatio();
		Vec3f cameraPosition = { 0.0f, 12.0f, -17.0f };
		Vec3f cameraTarget = { 0.0f, 3.0f, 0.0f };
		camera.init(cameraPosition, cameraTarget);
		camera.setPerspective(90.0f, aspectRatio, 1.0f, 1000.0f);
	   
		//ubo.projectionMatrix = camera.getProjectionMatrix().clone();
		ubo.projectionMatrix = camera.getHUDProjectionMatrix((float)viewport->getWidth(), (float)viewport->getHeight());
		ubo.viewMatrix = camera.getViewMatrix().clone();
		ubo.modelMatrix = {
			300.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 300.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			300.0f, 300.0f, 0.0f, 1.0f };
		
		//ubo.projectionMatrix[5] *= -1;
		
		uniformBuffer = new MemoryBuffer(device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(UniformBufferObject), &ubo);


		shader = Shader::createShader(device, "resources/shaders/vert.spv", "resources/shaders/frag.spv");
		
		size_t vertexBufferSize = sizeof(vertices[0]) * vertices.size();
		size_t indexBufferSize = sizeof(indices[0]) * indices.size();
		size_t bufferSize = vertexBufferSize + indexBufferSize;

		char *bufferData = (char*) malloc(bufferSize);
		std::memcpy(bufferData, vertices.data(), vertexBufferSize);
		std::memcpy(&bufferData[vertexBufferSize], indices.data(), indexBufferSize);

		dataBuffer = new MemoryBuffer(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bufferSize, bufferData);

		delete[] bufferData;

		createPipeline(device, swapChain, viewport);
	}

	void RendererObject::update(long long elapsedTime) 
	{
		camera.update(elapsedTime);

		float aspectRatio = viewport->getAspectRatio();
		camera.setPerspective(90.0f, aspectRatio, 1.0f, 1000.0f);

		ubo.projectionMatrix = camera.getProjectionMatrix().clone();
		ubo.viewMatrix = camera.getViewMatrix().clone();
		ubo.modelMatrix = { 
			1.0f, 0.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 1.0f, 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f };

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffer->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);
		
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = graphicPipeline->descriptorSets[0];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional 
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device->logicalDevice, 1, &descriptorWrite, 0, nullptr);
	}

	void RendererObject::createPipeline(Device* device, SwapChain* swapChain, Viewport* viewport) 
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

		graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport, &vertexInputInfo);
	}

	void RendererObject::render(CommandManager* commandManager, SwapChain* swapChain, uint32_t imageIndex, Viewport* viewport)
	{
		VkBuffer vertexBuffers[] = { dataBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		size_t vertexBufferSize = sizeof(vertices[0]) * vertices.size();

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

		if (uniformBuffer != nullptr)
		{
			delete uniformBuffer;
			uniformBuffer = nullptr;
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