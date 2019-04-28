#include "Window.h"
#include "GL.h"
#include <iostream>
#include <windows.h>
#include <thread>

using namespace std;

static const int NUM_KEYS = 512;

bool l_pressed[NUM_KEYS];
bool pressed[NUM_KEYS];
bool held[NUM_KEYS];

int mouseLastPosX = 0, mousePosX = 0, mouseBufferPosX;
int mouseLastPosY = 0, mousePosY = 0, mouseBufferPosY;

void(*onWindowClose)() = nullptr;


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
		CW_USEDEFAULT, CW_USEDEFAULT, 1440, 900,

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
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};


	int idPixel = ChoosePixelFormat(contextDevice, &pfd);

	SetPixelFormat(contextDevice, idPixel, &pfd);

	ShowWindow(hwnd, 5);
	initialized = true;
	
	//ShowCursor(false);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::updateInput(){
	memcpy((void *)pressed, (void *)l_pressed, NUM_KEYS);
	memset((void *)l_pressed, 0, NUM_KEYS);
	
	if (GetForegroundWindow() == hwnd) {
		//if(lockWindow){
		mouseLastPosX = 720;
		mouseLastPosY = 450;
		//}
		//else
		//{
		//mouseLastPosX = mousePosX;
		//mouseLastPosY = mousePosY;
		//}
		mousePosX = mouseBufferPosX;
		mousePosY = mouseBufferPosY;

		POINT pt;
		pt.x = 720;
		pt.y = 450;
		ClientToScreen(hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}
}

bool Window::isHeld(int key)
{
	return held[key];
}

bool Window::isPressed(int key)
{
	return pressed[key];
}

int Window::getMouseXDelta() {
	return (mousePosX - mouseLastPosX);
}
int Window::getMouseYDelta() {
	return (mouseLastPosY - mousePosY);
}

void Window::setOnWindowClose(void(*func)())
{
	onWindowClose = func;
}

Window::Window()
{

}


Window::~Window()
{
	
}

struct MouseMove {
	short x;
	short y;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		if (onWindowClose) { onWindowClose(); }
		return 0;
	case WM_DESTROY:
		return 0;

#pragma region INPUT/OUTPUT
	case WM_KEYDOWN:
		if (wParam < NUM_KEYS && !(lParam & 0xFF000000)) {
			l_pressed[wParam] = true;
			held[wParam] = true;
		}
		break;
	case WM_KEYUP:
		if (wParam < NUM_KEYS) {
			held[wParam] = false;
		}
		break;
	case WM_MOUSEMOVE:
		MouseMove *m = (MouseMove *)&lParam;
		mouseBufferPosX = m->x;
		mouseBufferPosY = m->y;
		break;
#pragma endregion



	}




	return DefWindowProc(hwnd, uMsg, wParam, lParam);	//Does default stuff (red X means quit, return early to avoid this)

}


