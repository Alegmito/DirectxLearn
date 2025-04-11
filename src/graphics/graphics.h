#pragma once
#include <d3d11.h>
#include <Windows.h>

#pragma comment(lib, "d3d11.lib")

class Graphics {
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics &operator=(const Graphics&) = delete;
    ~Graphics();
    void createEndFrame();
    void clearbuffer (float r, float g, float b) noexcept {
        const float color[] = {r, g, b, 1.0f};
        context_->ClearRenderTargetView(renderTargetView_, color);
    }
private:
    void throwGraphicsError(HRESULT hr);
    ID3D11Device *device_ {};
    IDXGISwapChain *swap_ {};
    ID3D11DeviceContext *context_ {};
    ID3D11RenderTargetView *renderTargetView_ {};

};
