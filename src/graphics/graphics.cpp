#include "graphics.h"
#include "exceptions/graphicsException.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <array>

#define GFX_EXCEPT_NOINFO(hr) HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_NOINFO(hrCall) if (FAILED(hrCall)) throw HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_REMOVED_EXCEPT(hr) HResultException( __LINE__,__FILE__,hr )

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


#ifndef NDEBUG
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#define GFX_THROW_INFO(hrcall) infoManager_.set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#define GFX_THROW_INFO_ONLY(call) infoManager_.set(); (call); {auto v = infoManager_.getLog(); if (!v.empty()) {throw InfoException(__LINE__, __FILE__, v);}}
#else
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

namespace mWrl = Microsoft::WRL;

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

void Graphics::drawTestTriangle() {
    struct Vertex {
        float x;
        float y;
    };

    std::array<Vertex, 3> vertices {
        {{0.0f,  0.5f},
            {0.5f, -0.5f},
            {-0.5f, -0.5f}}
    };


    mWrl::ComPtr<ID3D11Buffer> vertexBuffer {};
    auto buffDescr {D3D11_BUFFER_DESC {}};
    buffDescr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffDescr.Usage = D3D11_USAGE_DEFAULT;
    buffDescr.CPUAccessFlags = {};
    buffDescr.ByteWidth = sizeof(vertices);
    buffDescr.MiscFlags = {};
    buffDescr.StructureByteStride = sizeof(Vertex);

    auto subrscData {D3D11_SUBRESOURCE_DATA {}};
    subrscData.pSysMem = vertices.data();

    HRESULT hr {};
    GFX_THROW_INFO(
    device_->CreateBuffer(&buffDescr, &subrscData, &vertexBuffer)
    );

    // Bind vertex buffer to pipeline
    const UINT strides {sizeof(Vertex)};
    const UINT offsets {};
    context_->IASetVertexBuffers({}, 1, &vertexBuffer, &strides, &offsets);

    mWrl::ComPtr<ID3DBlob> blob {};
    // Have to create Vertex and Pixel shaders
    mWrl::ComPtr<ID3D11VertexShader> vertexShader {};
    GFX_THROW_INFO( D3DReadFileToBlob(L"vertexShader.cso", &blob));

    GFX_THROW_INFO(
        device_->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader)
    );

    context_->VSSetShader(vertexShader.Get(), nullptr, 0u);

    GFX_THROW_INFO_ONLY(context_->Draw(vertices.size(), 0u));
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
