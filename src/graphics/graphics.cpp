#include "graphics.h"
#include "exceptions/graphicsException.h"
#include <d3d11.h>
#include <d3d9types.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <array>
#include <DirectXMath.h>

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
}

using ConstBuffer = dx::XMMATRIX;
void Graphics::drawTestTriangle(float angle, float x, float z) {
    struct Position { float x; float y; float z; };

    struct Color { float r; float g; float b; float a; };
    struct Vertex { Position v; Color color; };

    std::array<Position, 8> vertices {{
        {-1.f, -1.f, -1.f},
        {1.f, -1.f, -1.f},
        {-1.f, 1.f, -1.f},
        {1.f, 1.f, -1.f},
        {-1.f, -1.f, 1.f},
        {1.f, -1.f, 1.f},
        {-1.f, 1.f, 1.f},
        {1.f, 1.f, 1.f}
    }};

    mw::ComPtr<ID3D11Buffer> vertexBuffer {};
    auto buffDescr {D3D11_BUFFER_DESC {}};
    buffDescr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffDescr.Usage = D3D11_USAGE_DEFAULT;
    buffDescr.CPUAccessFlags = {};
    buffDescr.ByteWidth = sizeof(vertices);
    buffDescr.MiscFlags = {};
    buffDescr.StructureByteStride = sizeof(Position);

    auto subrscData {D3D11_SUBRESOURCE_DATA {}};
    subrscData.pSysMem = vertices.data();

    HRESULT hr {};
    GFX_THROW_INFO(device_->CreateBuffer(&buffDescr, &subrscData, &vertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT strides {sizeof(Position)};
    const UINT offsets {};
    context_->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &strides, &offsets);

    const std::array<uint16_t, 3 * 2 * 6> indices {
        0, 2, 1,  2, 3, 1,
        1, 3, 5,  3, 7, 5,
        2, 6, 3,  3, 6, 7,
        4, 5, 7,  4, 7, 6,
        0, 4, 2,  2, 4, 6,
        0, 1, 4,  1, 5, 4
    };

    mw::ComPtr<ID3D11Buffer> indexBuffer {};
    auto iBuffDescr {D3D11_BUFFER_DESC {}};
    iBuffDescr.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBuffDescr.Usage = D3D11_USAGE_DEFAULT;
    iBuffDescr.CPUAccessFlags = {};
    iBuffDescr.ByteWidth = sizeof(indices);
    iBuffDescr.MiscFlags = {};
    iBuffDescr.StructureByteStride = sizeof(uint16_t);

    auto iSubrscData {D3D11_SUBRESOURCE_DATA {}};
    iSubrscData.pSysMem = indices.data();

    GFX_THROW_INFO(device_->CreateBuffer(&iBuffDescr, &iSubrscData, &indexBuffer));

    context_->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    // We need to create a constant buffer for the transformation matrix

    const ConstBuffer constBuffer { { dx::XMMatrixTranspose(
        dx::XMMatrixRotationZ(angle)
        * dx::XMMatrixRotationX(angle)
        * dx::XMMatrixTranslation(x * 4, 0, z + 4.f)
        * dx::XMMatrixPerspectiveLH(1.f, 3.f / 4.f, 0.5f, 10.f)
    ) } };

    mw::ComPtr<ID3D11Buffer> pConstBuffer {};

    D3D11_BUFFER_DESC constBufDescr {};
    constBufDescr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufDescr.Usage = D3D11_USAGE_DYNAMIC;
    constBufDescr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constBufDescr.ByteWidth = sizeof(constBuffer);
    constBufDescr.MiscFlags = {};
    constBufDescr.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA constBufSubSrcData {};
    constBufSubSrcData.pSysMem = &constBuffer;

    GFX_THROW_INFO(device_->CreateBuffer(&constBufDescr, &constBufSubSrcData, &pConstBuffer));
    context_->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());

    const std::array<Color, 6> colorBuffer {{
        {1.f, 0.f, 1.f, 0.f},
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {1.f, 1.f, 0.f, 0.f},
        {0.f, 1.f, 1.f, 0.f},
    }};


    mw::ComPtr<ID3D11Buffer> pColorBuffer {};

    D3D11_BUFFER_DESC colorBufDescr {};
    colorBufDescr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    colorBufDescr.Usage = D3D11_USAGE_DEFAULT;
    colorBufDescr.CPUAccessFlags = 0;
    colorBufDescr.ByteWidth = sizeof(colorBuffer);
    colorBufDescr.MiscFlags = {};
    colorBufDescr.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA colorBufSubSrcData {};
    colorBufSubSrcData.pSysMem = colorBuffer.data();

    GFX_THROW_INFO(device_->CreateBuffer(&colorBufDescr, &colorBufSubSrcData, &pColorBuffer));
    context_->PSSetConstantBuffers(0u, 1u, pColorBuffer.GetAddressOf());
    

    mw::ComPtr<ID3DBlob> vsBlob {}, psBlob {};
    // Have to create Vertex and Pixel shaders
    mw::ComPtr<ID3D11VertexShader> vertexShader {};
    GFX_THROW_INFO( D3DReadFileToBlob(L"vertexShader.cso", &vsBlob));
    GFX_THROW_INFO(
        device_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)
    );
    context_->VSSetShader(vertexShader.Get(), nullptr, 0u);

    mw::ComPtr<ID3D11PixelShader> pixelShader {};
    GFX_THROW_INFO( D3DReadFileToBlob(L"pixelShader.cso", &psBlob));
    GFX_THROW_INFO(
        device_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader)
    );
    context_->PSSetShader(pixelShader.Get(), nullptr, 0u);

    //
    // Define input layout
    
    mw::ComPtr<ID3D11InputLayout> inputLayout {};
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    GFX_THROW_INFO(
    device_->CreateInputLayout(layoutDesc, std::size(layoutDesc),
                              vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout)
    );
    context_->IASetInputLayout(inputLayout.Get());

    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    D3D11_VIEWPORT viewport {};
    viewport.Width = 800,
    viewport.Height = 600,
    viewport.MinDepth = 0,
    viewport.MaxDepth = 1,
    viewport.TopLeftX = 0,
    viewport.TopLeftY = 0;
    context_->RSSetViewports(1u, &viewport);

    GFX_THROW_INFO_ONLY(context_->DrawIndexed(indices.size(), 0u, 0u));
}

void Graphics::clearbuffer (float r, float g, float b) noexcept {
    const float color[] = {r, g, b, 1.0f};
    context_->ClearRenderTargetView(renderTargetView_.Get(), color);
    context_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
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
