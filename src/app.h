#pragma once;
#include "Window.h"

class App {
public:
    App();
    int Run();
private:
    void Tick();
    Window window_;
};
