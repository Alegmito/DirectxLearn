#include "window.h"
#include <basetsd.h>
#include <winuser.h>
#include <sstream>

Window::WindowConfig Window::WindowConfig::windowConfigSingleton;

Window::WindowConfig::WindowConfig() noexcept
    : hInst_(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = getInstance();
    wc.lpszClassName = getName();
    wc.hbrBackground = nullptr;

    if (!RegisterClassEx(&wc)) {
        MessageBox(nullptr, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return;
    };
}

Window::WindowConfig::~WindowConfig() {
    UnregisterClass(windowClassName.data(), getInstance());
}

const char *Window::WindowConfig::getName() noexcept {return windowClassName.data();}

HINSTANCE Window::WindowConfig::getInstance() noexcept { return windowConfigSingleton.hInst_; }

Window::Window(int width, int height, const char *name) noexcept {
    RECT wr {};
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.left;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

    HWND hWnd_ {CreateWindow(
        WindowConfig::getName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowConfig::getInstance(), this 
    )};

    ShowWindow(hWnd_, SW_SHOWDEFAULT);    
}

Window::~Window() {
    DestroyWindow(hWnd_);
}

LRESULT WINAPI Window::handleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        auto create {reinterpret_cast<CREATESTRUCTW *>(lParam)};
        auto window {static_cast<class Window *>(create->lpCreateParams)};
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMessageThunk));
        return window->handleMessage(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::handleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto pWnd {reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))};
    return pWnd->handleMessage(hWnd, msg, wParam, lParam);
}


LRESULT Window::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::string char_title;
    switch (msg) {
        case WM_CLOSE: {
        PostQuitMessage(0);
        return 0;
        }
        case WM_KEYDOWN: {
        if (wParam == 'F') {
            SetWindowText(hWnd, "Respects");
            break;
        }
        }
        case WM_KEYUP: {
        if (wParam == 'F') {
            SetWindowText(hWnd, "DangerField");
            SetWindowText(hWnd, char_title.c_str());
            break;
        }
        }
        case WM_CHAR: {
            char_title.push_back((char)wParam);
            SetWindowText(hWnd, char_title.c_str());
            break;
        }
        case WM_LBUTTONDOWN: {
            POINTS points = MAKEPOINTS(lParam);
            std::ostringstream oss {};
            oss <<  "(" << points.x << "," << points.y << ")";
            SetWindowText(hWnd, oss.str().c_str());
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
