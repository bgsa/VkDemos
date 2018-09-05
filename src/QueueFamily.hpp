#ifndef QUEUE_FAMILY_HEADER
#define QUEUE_FAMILY_HEADER

#include "VkBootstrapHeader.h"
#include "Queue.hpp"

namespace VkBootstrap 
{

	class QueueFamily 
	{
	private:
		uint32_t queueFamilyIndex = -1;
		std::vector<Queue*> queues;
		VkQueueFlagBits type = VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;
		bool presentationSupport = false;

	public:

		QueueFamily(VkQueueFlagBits type, uint32_t index, std::vector<Queue*> queues, bool presentationSupport)
		{
			this->type = type;
			this->queueFamilyIndex = index;
			this->queues = queues;
			this->presentationSupport = presentationSupport;
		}
		
		inline uint32_t getIndex() 
		{
			return queueFamilyIndex;
		}

		inline std::vector<Queue*> getQueues() 
		{
			return queues;
		}

		inline VkQueueFlagBits getType()
		{
			return type;
		}

		inline bool isGraphicFamily() 
		{
			return type & VK_QUEUE_GRAPHICS_BIT;
		}

		inline bool isComputeFamily()
		{
			return type & VK_QUEUE_COMPUTE_BIT;
		}

		inline bool isTransferFamily()
		{
			return type & VK_QUEUE_TRANSFER_BIT;
		}

		inline bool isSparseBindingFamily()
		{
			return type & VK_QUEUE_SPARSE_BINDING_BIT;
		}

		inline bool hasPresentationSupport() 
		{
			return presentationSupport;
		}

		~QueueFamily() 
		{
			for (Queue* queue : queues)
				delete queue;
		}
	};

}

#endif

