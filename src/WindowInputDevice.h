#ifndef WINDOW_INPUT_DEVICE
#define WINDOW_INPUT_DEVICE

#include <vector>

#include "InputDevice.h"
#include "WindowInputDeviceHandler.h"

namespace VkBootstrap
{

class WindowInputDevice : public InputDevice
{
  protected:
	std::vector<WindowInputDeviceHandler *> handlers;

  public:
	size_t getHandlersCount()
	{
		return handlers.size();
	}

	WindowInputDeviceHandler **getHandlers()
	{
		return handlers.data();
	}

	void addHandler(WindowInputDeviceHandler *handler)
	{
		handlers.push_back(handler);
	}

	void removeHandler(WindowInputDeviceHandler *handler)
	{
		std::vector<WindowInputDeviceHandler *>::const_iterator item = std::find(handlers.begin(), handlers.end(), handler);

		if (item != handlers.end())
			handlers.erase(item);
	}
};

}

#endif