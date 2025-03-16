#pragma once
#include "keyboardController.h"
#include <Windows.h>
#include <minwindef.h>
#include <string_view>

class Window {

private:
    class WindowConfig {
    public:
        static const char *getName() noexcept;
        static HINSTANCE getInstance() noexcept;
    private:
        WindowConfig() noexcept;
        ~WindowConfig();
        WindowConfig(const WindowConfig&) = delete;
        static constexpr std::string_view windowClassName {"Direct 3D engine window"};
        static WindowConfig windowConfigSingleton;
        HINSTANCE hInst_;
    };
public:
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window &) = delete;
    Window & operator=(const Window &) = delete;
    KeyboardController keyboard_ {};
private:
    static LRESULT CALLBACK handleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK handleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    int width_;
    int height_;
    HWND hWnd_;
};
