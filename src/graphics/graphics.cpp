#include "graphics.h"
#include "exceptions/graphicsException.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <intsafe.h>
#include <winerror.h>

 #define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,hr )
 #define GFX_THROW_REMOVED_EXCEPT(hr) HResultException( __LINE__,__FILE__,hr )

Graphics::Graphics(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc {};
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = {};
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    auto hr {D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swap_,
        &device_,
        nullptr, 
        &context_
    )};

    if (FAILED(hr)) {throw  GFX_THROW_REMOVED_EXCEPT(hr);}

    ID3D11Resource *backBuffer {};
    hr = swap_->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
    if (FAILED(hr)) {throw  GFX_THROW_REMOVED_EXCEPT(hr);}

    hr = device_->CreateRenderTargetView(backBuffer, {}, &renderTargetView_);
    if (FAILED(hr)) {throw  GFX_THROW_REMOVED_EXCEPT(hr);}
    backBuffer->Release();
}


Graphics::~Graphics() {
    if (renderTargetView_) {renderTargetView_->Release();}
    if (swap_) {swap_->Release(); }
    if (context_) {context_->Release(); }
    if (device_) {device_->Release(); }
}

void Graphics::createEndFrame() {
    HRESULT hr {};
    if (FAILED(hr = swap_->Present(1u, 0u))) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(hr);
        } else {
            if (FAILED(hr)) {throw  GFX_THROW_REMOVED_EXCEPT(hr);}
        }
    }
}
