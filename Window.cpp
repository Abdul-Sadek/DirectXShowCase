#include "Window.h"
#include <exception>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;
    // (Your code process Win32 messages)

    switch (msg) {
    case WM_CREATE:
        break;
    case WM_SIZE:
        if (window) window->onSize();
        break;
    case WM_SETFOCUS:
        if (window) window->onFocus();
        break;
    case WM_KILLFOCUS:
        if (window) window->onKillFocus();
        break;
    case WM_DESTROY:
        if (window) window->onDestroy();
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}

Window::Window()
    : hwnd(nullptr), is_running(false), initialized(false)
{
    // Initialize WNDCLASSEX structure
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = L"MyTaylorClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Register window class
    if (!::RegisterClassEx(&wc)) {
        throw std::exception("Window class couldn't be created!");
    }

    // Create window
    hwnd = ::CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        L"MyTaylorClass",
        L"THE TORTURED POETS DEPARTMENT",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        nullptr,
        nullptr,
        wc.hInstance,
        this
    );

    // Check if window creation succeeded
    if (!hwnd) {
        throw std::exception("Window couldn't be created!");
    }

    // Show and update the window
    ::ShowWindow(hwnd, SW_SHOW);
    ::UpdateWindow(hwnd);
    is_running = true;
}

bool Window::broadcast()
{
    MSG msg;
    if (!initialized) {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        onCreate();
        initialized = true;
    }

    onUpdate();

    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(1);

    return true;
}

bool Window::isRun()
{
    if (is_running) {
        broadcast();
    }
    return is_running;
}

RECT Window::getClientWindowSizeRect()
{
    RECT rc;
    ::GetClientRect(hwnd, &rc);
    return rc;
}

RECT Window::getScreenSizeRect()
{
    RECT rc;
    rc.right = ::GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
    return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
    is_running = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onSize()
{
}

Window::~Window()
{
    if (hwnd) {
        ::DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}
