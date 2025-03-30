#pragma once
#include "directxTimer.h"
#include "window.h"

class App {
public:
    App();
    int Run();
private:
    void Tick();
    Window window_;
    DirectxTimer timer {};
};
