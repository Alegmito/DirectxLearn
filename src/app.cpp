#include "app.h"
#include "Pyramid.h"
#include "window.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <cmath>
#include <memory>
#include <optional>
#include <random>
#include <sstream>
#include "graphics/drawable/Box.h"

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
{
    auto sharedBindsMutex {std::make_shared<std::mutex>()};
    auto boxSharedBinds {std::make_shared<SharedBinds>()};
    boxSharedBinds->SetMutex(sharedBindsMutex);

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> adist{0.f, 3.1415f * 2.f},
                                          ddist{0.f, 3.1415f * 2.f},
                                          odist{0.f, 3.1415f * 0.3f},
                                          rdist{6.f, 20.f} ;

    for (size_t i = 0; i < 80; i++) {
        drawables_.push_back(std::make_unique<Box>(
            window_.getGraphics(), rng, adist, ddist, odist, rdist, boxSharedBinds
        ));
        drawables_[i]->Init();
    }

    window_.getGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.f, 3.f / 4.f, 0.4f, 50.f));
}

int App::Run() {
    for (std::optional<int> eCode {std::nullopt};;) {
        if (eCode = Window::processMessage(); eCode.has_value()) {return eCode.value();}
        Tick();
    }
}

App::~App() {
}

void App::Tick() {
    const float sinTime = sin(timer.peek()) / 2.0f + 0.5f;
    auto &graphics {window_.getGraphics()};
    const auto deltaTime {timer.mark()};
    graphics.clearbuffer(sinTime, sinTime, 1.0f);
    for (auto& box : drawables_) {
        box->Update(deltaTime);
        box->Draw(window_.getGraphics());
    }
    graphics.createEndFrame();
}

ShapesFactory::ShapesFactory(Graphics & graphics)
: graphics_ {graphics} {
    
    
}

std::unique_ptr<Drawable> ShapesFactory::operator()() {
    switch (typedist_(rng_)) {
        case 0:
            return std::make_unique<Pyramid> (
                graphics_, rng_, adist_, ddist_, odist_, rdist_
            );
        case 1:
            return std::make_unique<Box> (
                graphics_, rng_, adist_, ddist_, odist_, rdist_, bdist_
            );
        case 2:
            return std::make_unique<Melon> (
                graphics_, rng_, adist_, ddist_, odist_, rdist_, bdist_
            );
    
    }

}
