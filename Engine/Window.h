#pragma once

#include <thread>
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	HDC getContextDevice() { return contextDevice; }
	bool isInitialized() { return initialized; }
	void initialize();

private:
	void threadInit();


	std::thread listenThread;
	volatile bool initialized = false;
	HWND hwnd;
	HDC contextDevice;

};

