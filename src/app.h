#pragma once
#include "window.h"

class App {
public:
    App();
    int Run();
private:
    void Tick();
    Window window_;
};
