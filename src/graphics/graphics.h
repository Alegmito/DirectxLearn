#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../logger/dxgiLogger.h"
#include <wrl.h>
#include <wrl/client.h>

class Graphics {
public:

    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics &operator=(const Graphics&) = delete;
    virtual ~Graphics() {};
    void createEndFrame();
    void clearbuffer(float r, float g, float b) noexcept;
    void drawTestTriangle(float angle = 0.f);
private:
    void throwGraphicsError(HRESULT hr);
    Microsoft::WRL::ComPtr<ID3D11Device> device_ {};
    Microsoft::WRL::ComPtr<IDXGISwapChain> swap_ {};
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_ {};
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_ {};

    #ifndef NDEBUG
    DxgiInfo infoManager_ {};
    #endif
};
