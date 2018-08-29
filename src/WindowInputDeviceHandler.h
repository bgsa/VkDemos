#pragma once

class WindowInputDeviceHandler
{
public:
	virtual void window_OnClose(){};
	virtual void window_OnResize(int width, int height){};

	virtual ~WindowInputDeviceHandler(){};
};
