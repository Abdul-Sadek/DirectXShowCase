#pragma once
#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_win32.h"
#include "imgui\\imgui_impl_dx11.h"
#include <Windows.h>

class Window
{
public:
	//initialize the window
	Window();
	bool isRun();
	//to get window size
	RECT getClientWindowSizeRect();
	//to get screen size
	RECT getScreenSizeRect();
	//events
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();
	//release the window
	~Window();
private:
	bool broadcast();
protected:
	HWND hwnd;
	bool is_running;
	bool initialized = false;
};