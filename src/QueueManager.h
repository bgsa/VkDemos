#ifndef QUEUE_MANAGER_HEADER
#define QUEUE_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "QueueFamily.hpp"

namespace VkBootstrap
{
class QueueManager
{
private:
	std::vector<QueueFamily*> queuesFamily;

public:
	QueueManager(const VkPhysicalDevice &physicalDevice, const VkDevice& logicalDevice, const VkSurfaceKHR& surface);

	inline std::vector<QueueFamily*> getQueuesFamily();
	QueueFamily* getGraphicQueueFamily();
	QueueFamily* getTransferQueueFamily();
	QueueFamily* getComputeQueueFamily();
	QueueFamily* getSparseBindingFamily();
	QueueFamily* getPresentationQueueFamily();

	static std::vector<VkQueueFamilyProperties> getQueueFamilies(const VkPhysicalDevice &device);
	static bool hasGraphicQueueFamily(std::vector<VkQueueFamilyProperties> queueFamilies);
	static bool hasGraphicQueueFamily(const VkPhysicalDevice &device);
	static bool isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily);
	static uint32_t getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice);
	static uint32_t getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);

	~QueueManager();
};
}

#endif