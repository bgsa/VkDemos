#ifndef MEMORY_BUFFER_HEADER
#define MEMORY_BUFFER_HEADER

#include "VkBootstrapHeader.h"
#include "Device.h"
#include "VkPhysicalDeviceManager.h"

namespace VkBootstrap
{

	class MemoryBuffer
	{
	private:
		Device* device = nullptr;
		VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

	public:
		MemoryBuffer(Device* device, VkBufferUsageFlags usage, VkDeviceSize bufferSize, void* buffer);

		VkBuffer getBuffer();

		~MemoryBuffer();
	};

}

#endif