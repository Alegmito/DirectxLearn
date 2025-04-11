#include "app.h"
#include "window.h"
#include <Windows.h>
#include <cmath>
#include <iomanip>
#include <ios>
#include <optional>
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
            ++i;
            oss << "WheelUp: " << i;
            window.setTitle(oss.str());
            break;
        }
        case MouseEvent::Type::wheelDown: {
            std::ostringstream oss {};
            --i;
            oss << "WheelDown: " << i;
            window.setTitle(oss.str());
            break;
        }
    }
}

constexpr auto pClassName {"DirectX Learn"};
App::App()
    : window_(800, 600, pClassName)
{}

int App::Run() {
    for (std::optional<int> eCode {std::nullopt};;) {
        if (eCode = Window::processMessage(); eCode.has_value()) {return eCode.value();}
        Tick();
    }
}

void App::Tick() {
    // std::ostringstream oss {};
    // oss << "Delta time: "<< std::setprecision(2) << std::fixed << timer.peek() << "s";
    // window_.setTitle(oss.str());
    const float sinTime = sin(timer.peek()) / 2.0f + 0.5f;
    window_.getGraphics().clearbuffer(sinTime, sinTime, 1.0f);
    window_.getGraphics().createEndFrame();
}
