#include <Windows.h>
#include <minwindef.h>
#include <sstream>
#include <windef.h>
#include <winuser.h>
#include "controls/mouseEvent.h"
#include "exceptions/directxException.h"
#include "window.h"

void processMouse(Window &window) {
    const auto e {window.mouse_.Read()};
    if (!e.has_value()) {
        return;
    }

    static int i {};
    switch (e.value().getType()) {
        case MouseEvent::Type::leave:
            window.setTitle("Gone!");
            break;
        case MouseEvent::Type::move: {
            std::ostringstream oss {};
            oss << "Mouse moved to (" << window.mouse_.getState().x << "," << window.mouse_.getState().y << ")";
            window.setTitle(oss.str());
            break;
        }
        case MouseEvent::Type::wheelUp: {
            std::ostringstream oss {};
            oss << "WheelUp: " << i;
            window.setTitle(oss.str());
        }
        case MouseEvent::Type::wheelDown: {
            std::ostringstream oss {};
            oss << "WheelDown: " << i;
            window.setTitle(oss.str());
        }
    }
}

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
            if (!window.mouse_.isEmpty()) {
                processMouse(window);
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
