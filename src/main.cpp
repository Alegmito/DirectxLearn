#include <Windows.h>
#include <minwindef.h>
#include <windef.h>
#include <winuser.h>
#include "exceptions/directxException.h"
#include "window.h"

constexpr auto pClassName {"directxLearn"};
int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) {
    try {

        Window window {800, 600, pClassName};
        MSG message {};
        BOOL gResult {};

        while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            if (window.keyboard_.isKeyPressed(VK_MENU)) {
                MessageBox(nullptr, "Something happenned!", "Alt key was pressed", MB_OK | MB_ICONEXCLAMATION);
            }
        }

        if (gResult == 1) {
            return -1;
        }

        return message.wParam;
    }
    catch (DirectxException &ex) {
        MessageBox(nullptr, ex.what(), ex.getType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (std::exception &ex) {
        MessageBox(nullptr, ex.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, "No details avaliable", "Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;

}
