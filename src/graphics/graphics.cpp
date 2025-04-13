#include "graphics.h"
#include "exceptions/graphicsException.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <intsafe.h>
#include <winerror.h>
#include <wrl/client.h>

#define GFX_EXCEPT_NOINFO(hr) HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_NOINFO(hrCall) if (FAILED(hrCall)) throw HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_REMOVED_EXCEPT(hr) HResultException( __LINE__,__FILE__,hr )


#ifndef NDEBUG
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#define GFX_THROW_INFO(hrcall) infoManager_.set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#else
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

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
    
    UINT swapCreateFlags {};
    #ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    HRESULT hr {};
    
    GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swap_,
        &device_,
        nullptr, 
        &context_
    ));

    mWrl::ComPtr<ID3D11Resource> backBuffer {};
    GFX_THROW_INFO(
        swap_->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer)
    );
    GFX_THROW_INFO(
        device_->CreateRenderTargetView(backBuffer.Get(), {}, &renderTargetView_)
    );
}

void Graphics::clearbuffer (float r, float g, float b) noexcept {
    const float color[] = {r, g, b, 1.0f};
    context_->ClearRenderTargetView(renderTargetView_.Get(), color);
}

void Graphics::createEndFrame() {
    HRESULT hr {};
    #ifndef NDEBUG
    infoManager_.set();
    #endif

    if (FAILED(hr = swap_->Present(1u, 0u))) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(hr);
        } else {
            throw GFX_EXCEPT(hr);
        }
    }
}
