#include "Renderer.h"

namespace VkBootstrap
{
	Renderer* instance = nullptr;

	Renderer::Renderer(Device* device, SwapChain* swapChain, Size windowSize)
	{
		this->device = device;
		this->viewport = new Viewport(windowSize.width, windowSize.height);
		setSwapChain(swapChain);

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
		memoryBufferBarriers.push_back(uniformBuffer->createBarrier());


		createDescriptorPool(device);
		createDescriptorSetLayout(device);
		createDescriptorSets(device);

		rendererObject = new RendererObject(device, swapChain, viewport, &descriptorSetLayout, descriptorSets);
	}

	void Renderer::init(Device* device, SwapChain* swapChain, Size windowSize)
	{
		if (instance != nullptr)
			delete instance;

		instance = new Renderer(device, swapChain, windowSize);
	}

	Renderer* Renderer::getInstance() 
	{
		return instance;
	}

	void Renderer::setSwapChain(SwapChain* swapChain) 
	{
		this->swapChain = swapChain;
	}

	void Renderer::resize(int width , int height) 
	{
		viewport->setSize(width, height);
	}

	void Renderer::createDescriptorPool(const Device* device)
	{
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = 1;

		if (vkCreateDescriptorPool(device->logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor pool!");
	}

	void Renderer::createDescriptorSetLayout(const Device* device)
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(device->logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");
	}

	void Renderer::createDescriptorSets(const Device* device)
	{
		descriptorSets.resize(1);
		std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(device->logicalDevice, &allocInfo, &descriptorSets[0]) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
	}

	void Renderer::update(long long elapsedTime)
	{
		camera.update(elapsedTime);

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffer->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(float) * 16 * 3;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[0];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional 
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device->logicalDevice, 1, &descriptorWrite, 0, nullptr);
		
		rendererObject->update(elapsedTime);
	}

	void Renderer::render(uint32_t imageIndex)
	{
		rendererObject->render(swapChain, imageIndex);
	}

	Renderer::~Renderer()
	{
		if (uniformBuffer != nullptr)
		{
			delete uniformBuffer;
			uniformBuffer = nullptr;
		}

		if (descriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(device->logicalDevice, descriptorPool, nullptr);
			descriptorPool = VK_NULL_HANDLE;
		}

		if (descriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(device->logicalDevice, descriptorSetLayout, nullptr);
			descriptorSetLayout = VK_NULL_HANDLE;
		}

		if (rendererObject != nullptr)
		{
			delete rendererObject;
			rendererObject = nullptr;
		}

		if (viewport != nullptr)
		{
			delete viewport;
			viewport = nullptr;
		}
	}
	
}