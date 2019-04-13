#include "Window.h"
#include "GL.h"
#include <iostream>
#include <windows.h>
#include <thread>

using namespace std;



MSG msg = {};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


void Window::initialize() {
	listenThread = thread(&Window::threadInit, this);
	listenThread.detach();
}

void Window::threadInit() {
	const char CLASS_NAME[] = "Empty Window";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);


	hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		"Dominic's Program",    // Window text
		CS_OWNDC/* | WS_OVERLAPPEDWINDOW  | WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/,            // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		wc.hInstance,  // Instance handle
		NULL        // Additional application data
	);




	//OpenGL context binding.
	contextDevice = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,                   // Number of bits for the depthbuffer
		0,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};


	int idPixel = ChoosePixelFormat(contextDevice, &pfd);

	SetPixelFormat(contextDevice, idPixel, &pfd);

	ShowWindow(hwnd, 5);
	initialized = true;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Window::Window()
{

}


Window::~Window()
{
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		exit(0);
		return 0;
	case WM_DESTROY:
		return 0;

#pragma region INPUT/OUTPUT
	case WM_KEYDOWN:

		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			exit(0);
			return 0;
		}

		break;
#pragma endregion



	}




	return DefWindowProc(hwnd, uMsg, wParam, lParam);	//Does default stuff (red X means quit, return early to avoid this)

}


