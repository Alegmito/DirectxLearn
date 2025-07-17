#pragma once
#include "directxTimer.h"
#include "window.h"
#include <vector>

class App {
public:
    App();
    ~App();
    int Run();
private:
    void Tick();
    Window window_;
    DirectxTimer timer {};
    std::vector<std::unique_ptr<class Box>> boxes_;
};
