#include <Windows.h>
#include <minwindef.h>
#include <sstream>
#include <string>
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include "window.h"

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::string char_title;
    switch (msg) {
        case WM_CLOSE: {
        PostQuitMessage(69);
        break;
        }
        case WM_KEYDOWN: {
        //if (wParam == 'F') {
        //    SetWindowText(hWnd, "Respects");
        //    break;
        //}
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

constexpr auto pClassName {"directxLearn"};
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR , int ) {
    
    WNDCLASSEX wc {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.lpszClassName = pClassName;
    wc.hbrBackground = nullptr;

    if (!RegisterClassEx(&wc)) {
        MessageBox(nullptr, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    };

    HWND hWnd {CreateWindowEx(
        0, pClassName,
        "Directx Learn Window",
        //WS_OVERLAPPEDWINDOW,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 640, 480,
        nullptr, nullptr, hInstance, nullptr
    )};

    if (hWnd == nullptr) {
        MessageBox(nullptr, "Window Creation failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG message {}; 
    BOOL gResult {};
    while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    if (gResult == -1) {
        return gResult;
    } else {
        return message.wParam;
    }
}
