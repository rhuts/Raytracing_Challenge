#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window(int width, int height);
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	~Window();

	bool ProcessMessages();
	HWND GetHWindow() { return m_hWnd; }
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_width;
	int m_height;
};

#endif