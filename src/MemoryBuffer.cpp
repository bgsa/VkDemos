#include "MemoryBuffer.h"

namespace VkBootstrap
{

	void MemoryBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		uint32_t queuesFamily[2] = {
			device->queueManager->getGraphicQueueFamily()->getIndex(),
			device->queueManager->getTransferQueueFamily()->getIndex()
		};

		VkBufferCreateInfo bufferInfo = {}; 
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;  
		bufferInfo.size = size; 
		bufferInfo.usage = usage; 
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		bufferInfo.queueFamilyIndexCount = 2;
		bufferInfo.pQueueFamilyIndices = queuesFamily;
		
		if (vkCreateBuffer(device->logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
			throw std::runtime_error("failed to create buffer!"); 

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device->logicalDevice, buffer, &memoryRequirements);

		//find memory type
		VkPhysicalDeviceMemoryProperties memoryProperties = VkPhysicalDeviceManager::getMemoryProperties(device->physicalDevice);
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
				
		if (vkAllocateMemory(device->logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate buffer memory!");
		
		vkBindBufferMemory(device->logicalDevice, buffer, bufferMemory, 0);
	}

	void MemoryBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
	{
		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;

		CopyCommand* copyCommand = CommandManager::getInstance()->createCopyCommand();
		copyCommand->begin(size, 0, 0);
		copyCommand->execute(srcBuffer, dstBuffer, 1, &copyRegion);
		copyCommand->end();
	}

	MemoryBuffer::MemoryBuffer(Device* device, VkBufferUsageFlags usage, VkDeviceSize bufferSize, void* buffer)
	{
		this->device = device;

		VkBuffer stagingBuffer; 
		VkDeviceMemory stagingBufferMemory; 

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory); 
		
		void* data;
		vkMapMemory(device->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data); 
		memcpy(data, buffer, (size_t)bufferSize); 
		vkUnmapMemory(device->logicalDevice, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory); 
				
		copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
						
		vkDestroyBuffer(device->logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(device->logicalDevice, stagingBufferMemory, nullptr);
	}

	VkBuffer MemoryBuffer::getBuffer()
	{
		return vertexBuffer;
	}

	VkBufferMemoryBarrier MemoryBuffer::createBarrier()
	{
		memoryBufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		memoryBufferBarrier.pNext = nullptr;
		memoryBufferBarrier.buffer = vertexBuffer;
		memoryBufferBarrier.size = VK_WHOLE_SIZE;
		memoryBufferBarrier.offset = 0;
		memoryBufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memoryBufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memoryBufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		memoryBufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		//memoryBufferBarrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;
		
		return memoryBufferBarrier;
	}

	MemoryBuffer::~MemoryBuffer()
	{
		vkDestroyBuffer(device->logicalDevice, vertexBuffer, nullptr);
		vkFreeMemory(device->logicalDevice, vertexBufferMemory, nullptr);
	}

}