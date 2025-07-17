#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../logger/dxgiLogger.h"
#include "utils.hpp"
#include <wrl.h>
#include <wrl/client.h>
#include <DirectXMath.h>

class Graphics {
public:

    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics &operator=(const Graphics&) = delete;
    virtual ~Graphics() {};
    void createEndFrame();
    void clearbuffer(float r, float g, float b) noexcept;
    void DrawIndexed(UINT count) NOEXCEPT;

    ID3D11Device* getDevice() {return device_.Get();}
    ID3D11DeviceContext* getContext() {return context_.Get();}
    DxgiInfo& getInfoManager() {
#ifndef NDEBUG
        return infoManager_;
#else
        throw std::logic_error ("Error trying to access infoManager in Release");   
#endif
    }
    
private:
    void throwGraphicsError(HRESULT hr);
    Microsoft::WRL::ComPtr<ID3D11Device> device_ {};
    Microsoft::WRL::ComPtr<IDXGISwapChain> swap_ {};
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_ {};
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_ {};
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_ {};

public:
    void SetProjection (DirectX::XMMATRIX projection) {projection_ = projection;}
    DirectX::XMMATRIX GetProjection () {return projection_;};

private:
    DirectX::XMMATRIX projection_;
    
#ifndef NDEBUG
    DxgiInfo infoManager_ {};
#endif
};
