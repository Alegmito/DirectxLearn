#include "window.h"
#include "exceptions/windowException.h"
#include "resources.h"
#include <basetsd.h>
#include <errhandlingapi.h>
#include <wingdi.h>
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
    wc.hIcon = LoadIcon(getInstance(), MAKEINTRESOURCE(DIRECTX_APP_ICON));
    wc.hIconSm = LoadIcon(getInstance(), MAKEINTRESOURCE(DIRECTX_APP_ICON));

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

void Window::setTitle(const std::string &title) {
    if (!SetWindowText(hWnd_, title.c_str())) {
        throw GetLastError();
    }
}

Window::Window(int width, int height, const char *name) 
    : width_(width), height_(height)
{
    RECT wr {};
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.left;
    if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false)) {
        throw WindowException();
    }

    hWnd_ = CreateWindow(
        WindowConfig::getName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowConfig::getInstance(), this 
    );

    if (hWnd_ == nullptr) {
        throw WindowException();
    }

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
        case WM_KILLFOCUS:
            keyboard_.clearState();
            break;
        case WM_CLOSE: {
        PostQuitMessage(0);
        return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            if ((lParam & 0x40000000) == 1 && !keyboard_.isAutorepeat()) 
                break;

            if (wParam == 'F') {
                SetWindowText(hWnd, "Respects");
            }
            keyboard_.onKeyPressed(wParam);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            if (wParam == 'F') {
                SetWindowText(hWnd, "DangerField");
                SetWindowText(hWnd, char_title.c_str());
            }
            keyboard_.onKeyReleased(wParam);
            break;
        }
        case WM_CHAR: {
            char_title.push_back((char)wParam);
            SetWindowText(hWnd, char_title.c_str());
            keyboard_.onChar(wParam);
            break;
        }
        case WM_LBUTTONDOWN: {
            POINTS points = MAKEPOINTS(lParam);
            std::ostringstream oss {};
            oss <<  "(" << points.x << "," << points.y << ")";
            SetWindowText(hWnd, oss.str().c_str());
            mouse_.onLeftPressed(points.x, points.y);
            break;
        }
        case WM_MOUSEMOVE: {
            auto points {MAKEPOINTS(lParam)};
            if (points.x >= 0 && points.x < width_ && points.y >= 0 && points.y < height_) {
                mouse_.onMouseMove(points.x, points.y);
                if (!mouse_.isInWindow()) {
                    SetCapture(hWnd_);
                    mouse_.onMouseEnter();
                }

            } else {
                if (mouse_.getState().isLeftPressed || mouse_.getState().isRightPressed) {
                    mouse_.onMouseMove(points.x, points.y);
                } else {
                    ReleaseCapture();
                    mouse_.onMouseLeave();
                }
            }
            break;
        }
        case WM_LBUTTONUP: {
            auto points {MAKEPOINTS(lParam)};
            mouse_.onLeftReleased(points.x, points.y);
            break;
        }
        case WM_RBUTTONUP: {
            auto points {MAKEPOINTS(lParam)};
            mouse_.onRightReleased(points.x, points.y);
            break;
        }
        case WM_RBUTTONDOWN: {
            auto points {MAKEPOINTS(lParam)};
            mouse_.onRightPressed(points.x, points.y);
            break;
        }
        case WM_MOUSEWHEEL: {
            auto points {MAKEPOINTS(lParam)};
            const auto wheelDelta {GET_WHEEL_DELTA_WPARAM(wParam)};
            mouse_.onWheelDelta(points.x, points.y, wheelDelta);
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::processMessage() {
    MSG msg {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}
