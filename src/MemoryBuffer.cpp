#include "MemoryBuffer.h"

namespace VkBootstrap
{

	MemoryBuffer::MemoryBuffer(Device* device, VkBufferUsageFlags usage, VkDeviceSize bufferSize, void* buffer)
	{
		this->device = device;
		
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device->logicalDevice, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to create vertex buffer!");

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device->logicalDevice, vertexBuffer, &memoryRequirements);
		
		//find memory type
		VkPhysicalDeviceMemoryProperties memoryProperties = VkPhysicalDeviceManager::getMemoryProperties(device->physicalDevice);
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		int memoryTypeIndex = -1; 
		uint32_t typeFilter = memoryRequirements.memoryTypeBits;
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
			if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				memoryTypeIndex = i;
				break;
			}

		if (memoryTypeIndex == -1)
			throw std::runtime_error("failed to find suitable memory type!");

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
				
		if (vkAllocateMemory(device->logicalDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate vertex buffer memory!");

		vkBindBufferMemory(device->logicalDevice, vertexBuffer, vertexBufferMemory, 0);

		void* data;
		vkMapMemory(device->logicalDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);

		memcpy(data, buffer, (size_t)bufferInfo.size);

		vkUnmapMemory(device->logicalDevice, vertexBufferMemory);
	}

	VkBuffer MemoryBuffer::getBuffer() 
	{
		return vertexBuffer;
	}

	MemoryBuffer::~MemoryBuffer() 
	{
		vkDestroyBuffer(device->logicalDevice, vertexBuffer, nullptr);
		vkFreeMemory(device->logicalDevice, vertexBufferMemory, nullptr);
	}

}