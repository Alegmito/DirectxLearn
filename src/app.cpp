#include "app.h"
#include "Window.h"
#include <Windows.h>
#include <sstream>

void processMouse(Window &window) {
    const auto e {window.mouse_.Read()};
    if (!e.has_value()) {
        return;
    }

    static int i {};
    switch (e.value().getType()) {
        // case MouseEvent::Type::leave:
        //     window.setTitle("Gone!");
        //     break;
        // case MouseEvent::Type::move: {
        //     std::ostringstream oss {};
        //     oss << "Mouse moved to (" << window.mouse_.getState().x << "," << window.mouse_.getState().y << ")";
        //     window.setTitle(oss.str());
        //     break;
        // }
        case MouseEvent::Type::wheelUp: {
            std::ostringstream oss {};
            oss << "WheelUp: " << i;
            window.setTitle(oss.str());
            break;
        }
        case MouseEvent::Type::wheelDown: {
            std::ostringstream oss {};
            oss << "WheelDown: " << i;
            window.setTitle(oss.str());
            break;
        }
    }
}

App::App()
    : window_(800, 600, "DirectX Learn")
{}
int App::Run() {
    MSG message {};
    BOOL gResult {};

    while ((gResult = GetMessage(&message, nullptr, 0, 0)) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
        if (window_.keyboard_.isKeyPressed(VK_MENU)) {
            MessageBox(nullptr, "Something happenned!", "Alt key was pressed", MB_OK | MB_ICONEXCLAMATION);
        }
        if (!window_.mouse_.isEmpty()) {
            processMouse(window_);
        }
    }

    if (gResult == 1) {
        return -1;
    }

    return message.wParam;

}
void App::Tick() {
}
