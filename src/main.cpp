#include <Windows.h>
#include <minwindef.h>
#include <windef.h>
#include "window.h"

constexpr auto pClassName {"directxLearn"};
int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) {
    Window window {800, 300, pClassName};
    MSG message {};
    BOOL gResult {};

    while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    if (gResult == 1) {
        return -1;
    }

    return message.wParam;
}
