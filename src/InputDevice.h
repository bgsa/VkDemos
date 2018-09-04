#ifndef INPUT_DEVICE_HEADER
#define INPUT_DEVICE_HEADER

namespace VkBootstrap
{

class InputDevice
{
  public:
	virtual void update(long long elapsedTime) = 0;

	virtual ~InputDevice() {}
};

}

#endif