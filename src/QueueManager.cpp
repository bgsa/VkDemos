#include "QueueManager.h"

namespace VkBootstrap
{

	QueueManager::QueueManager(const VkPhysicalDevice &physicalDevice, const VkDevice& logicalDevice, const VkSurfaceKHR& surface)
	{
		std::vector<VkQueueFamilyProperties> vkFamilies = QueueManager::getQueueFamilies(physicalDevice);

		for (uint32_t i = 0; i != vkFamilies.size(); i++)
		{
			VkQueueFamilyProperties vkFamily = vkFamilies[i];
			std::vector<Queue*> queues;
			VkBool32 hasPresentationSupport = false;

			VkResult operationResult = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &hasPresentationSupport);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to query physical device surface support for queue");

			for (uint32_t j = 0; j < vkFamily.queueCount; j++)
			{
				VkQueue vkQueue = VK_NULL_HANDLE;
				vkGetDeviceQueue(logicalDevice, i, j, &vkQueue);

				Queue* queue = new Queue(j, vkQueue);
				queues.push_back(queue);
			}

			QueueFamily *queueFamily = new QueueFamily((VkQueueFlagBits)vkFamily.queueFlags, i, queues, hasPresentationSupport);
			queuesFamily.push_back(queueFamily);
		}
	}

	std::vector<QueueFamily*> QueueManager::getQueuesFamily()
	{
		return queuesFamily;
	}

	QueueFamily* QueueManager::getGraphicQueueFamily()
	{
		for (QueueFamily* queueFamily : queuesFamily)
			if (queueFamily->isGraphicFamily())
				return queueFamily;

		return nullptr;
	}

	QueueFamily* QueueManager::getTransferQueueFamily()
	{
		for (QueueFamily* queueFamily : queuesFamily)
			if (queueFamily->isTransferFamily())
				return queueFamily;

		return nullptr;
	}

	QueueFamily* QueueManager::getComputeQueueFamily()
	{
		for (QueueFamily* queueFamily : queuesFamily)
			if (queueFamily->isComputeFamily())
				return queueFamily;

		return nullptr;
	}

	QueueFamily* QueueManager::getSparseBindingFamily()
	{
		for (QueueFamily* queueFamily : queuesFamily)
			if (queueFamily->isSparseBindingFamily())
				return queueFamily;

		return nullptr;
	}

	QueueFamily* QueueManager::getPresentationQueueFamily()
	{
		for (QueueFamily* queueFamily : queuesFamily)
			if (queueFamily->hasPresentationSupport())
				return queueFamily;

		return nullptr;
	}

	std::vector<VkQueueFamilyProperties> QueueManager::getQueueFamilies(const VkPhysicalDevice &device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		return queueFamilies;
	}

	bool QueueManager::hasGraphicQueueFamily(std::vector<VkQueueFamilyProperties> queueFamilies)
	{
		for (const auto &queueFamily : queueFamilies)
			if (isGraphicQueueFamily(queueFamily))
				return true;

		return false;
	}

	bool QueueManager::hasGraphicQueueFamily(const VkPhysicalDevice &device)
	{
		std::vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilies(device);

		for (const auto &queueFamily : queueFamilies)
			if (isGraphicQueueFamily(queueFamily))
				return true;

		return false;
	}

	bool QueueManager::isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily)
	{
		return (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT);
	}

	uint32_t QueueManager::getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice)
	{
		std::vector<VkQueueFamilyProperties> queueFamilies = QueueManager::getQueueFamilies(physicalDevice);

		for (uint32_t i = 0; i != queueFamilies.size(); i++)
			if (QueueManager::isGraphicQueueFamily(queueFamilies[i]))
				return i;

		return -1;
	}

	uint32_t QueueManager::getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface)
	{
		std::vector<VkQueueFamilyProperties> queueFamilies = QueueManager::getQueueFamilies(physicalDevice);

		for (uint32_t i = 0; i != queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueCount == 0)
				return false;

			VkBool32 presentSupport = false;
			VkResult operationResult = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to get physical device surface support");

			if (presentSupport)
				return i;
		}

		return -1;
	}

	QueueManager::~QueueManager()
	{
		for (QueueFamily *queueFamily : queuesFamily)
			delete queueFamily;

		queuesFamily.clear();
	}

}
