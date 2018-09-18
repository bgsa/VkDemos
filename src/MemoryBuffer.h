#ifndef MEMORY_BUFFER_HEADER
#define MEMORY_BUFFER_HEADER

#include "VkBootstrapHeader.h"
#include "Device.h"
#include "CommandManager.h"

namespace VkBootstrap
{
	class MemoryBuffer
	{
	private:
		Device* device = nullptr;
		VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;
		VkBufferMemoryBarrier memoryBufferBarrier;

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	public:
		MemoryBuffer(Device* device, VkBufferUsageFlags usage, VkDeviceSize bufferSize, void* buffer);

		VkBuffer getBuffer();
		VkBufferMemoryBarrier createBarrier();

		~MemoryBuffer();
	};
}

#endif