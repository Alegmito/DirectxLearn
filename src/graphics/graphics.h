#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../logger/dxgiLogger.h"
#include <wrl.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")

    namespace mWrl = Microsoft::WRL;
class Graphics {
public:

    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics &operator=(const Graphics&) = delete;
    virtual ~Graphics() {};
    void createEndFrame();
    void clearbuffer(float r, float g, float b) noexcept;
private:
    void throwGraphicsError(HRESULT hr);
    mWrl::ComPtr<ID3D11Device> device_ {};
    mWrl::ComPtr<IDXGISwapChain> swap_ {};
    mWrl::ComPtr<ID3D11DeviceContext> context_ {};
    mWrl::ComPtr<ID3D11RenderTargetView> renderTargetView_ {};

    #ifndef NDEBUG
    DxgiInfo infoManager_ {};
    #endif
};
