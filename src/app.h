#pragma once
#include "Drawable.h"
#include "directxTimer.h"
#include "graphics.h"
#include "window.h"
#include <random>
#include <vector>
#include "GraphicsUtils.hpp"

class ShapesFactory{
public:
    ShapesFactory(Graphics & graphics);
    std::unique_ptr<Drawable> operator()();
private:
    std::mt19937 rng_ {std::random_device{}()};
    Graphics& graphics_;
    std::uniform_real_distribution<float> adist_ { 0.0f,pi_d * 2.0f };
    std::uniform_real_distribution<float> ddist_ { 0.0f,pi_d * 0.5f };
    std::uniform_real_distribution<float> odist_ { 0.0f,pi_d * 0.08f };
    std::uniform_real_distribution<float> rdist_ { 6.0f,20.0f };
    std::uniform_real_distribution<float> bdist_ { 0.4f,3.0f };
    std::uniform_int_distribution<int> latdist_  { 5,20 };
    std::uniform_int_distribution<int> longdist_ { 10,40 };
    std::uniform_int_distribution<int> typedist_ { 0,2 };
    std::vector<std::shared_ptr<SharedBinds>> sharedBindsList_ {};
};

class App {
public:
    App();
    ~App();
    int Run();
private:
    void Tick();
    Window window_;
    DirectxTimer timer {};
    std::vector<std::unique_ptr<class Drawable>> drawables_;
    static constexpr size_t nDrawables = 180;
};
