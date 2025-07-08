#include "graphics.h"
#include <d3d11.h>
#include <d3d9types.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "exceptions/graphicsExMacros.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace mw = Microsoft::WRL;
namespace dx = DirectX;

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

    mw::ComPtr<ID3D11Resource> backBuffer {};
    GFX_THROW_INFO(
        swap_->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer)
    );
    GFX_THROW_INFO(
        device_->CreateRenderTargetView(backBuffer.Get(), {}, &renderTargetView_)
    );

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    mw::ComPtr<ID3D11DepthStencilState> depthStencilState {};
    GFX_THROW_INFO(device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

    context_->OMSetDepthStencilState(depthStencilState.Get(), 1);

    mw::ComPtr<ID3D11Texture2D> depthStencilTexture {};
    D3D11_TEXTURE2D_DESC depthStencilTextureDesc {};
    depthStencilTextureDesc.Width  = 800;
    depthStencilTextureDesc.Height = 600;
    depthStencilTextureDesc.MipLevels = 1;
    depthStencilTextureDesc.ArraySize = 1;
    depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilTextureDesc.SampleDesc.Count = 1;
    depthStencilTextureDesc.SampleDesc.Quality = 0;
    depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    GFX_THROW_INFO(device_->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilTexture));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView {};
    descDepthStencilView.Format = DXGI_FORMAT_D32_FLOAT;
    descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDepthStencilView.Texture2D.MipSlice = 0;
    GFX_THROW_INFO(
        device_->CreateDepthStencilView(depthStencilTexture.Get(), &descDepthStencilView, &depthStencilView_)
    );

    context_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

    D3D11_VIEWPORT viewport {};
    viewport.Width = 800,
    viewport.Height = 600,
    viewport.MinDepth = 0,
    viewport.MaxDepth = 1,
    viewport.TopLeftX = 0,
    viewport.TopLeftY = 0;
    context_->RSSetViewports(1u, &viewport);
}

void Graphics::clearbuffer (float r, float g, float b) noexcept {
    const float color[] = {r, g, b, 1.0f};
    context_->ClearRenderTargetView(renderTargetView_.Get(), color);
    context_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Graphics::DrawIndexed(UINT count) NOEXCEPT{
    GFX_THROW_INFO_ONLY(context_->DrawIndexed(count, 0, 0));
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
