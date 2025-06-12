#pragma once
#include "../IBindable.h"
#include "../../exceptions/graphicsExMacros.h"
#include <d3d11.h>
#include <type_traits>
#include <wrl/client.h>

template <class C, typename ShaderType>
class ConstantBuffer : public IBindable {
public:
    ConstantBuffer(Graphics& gfx, const C& consts) {
        INFOMANAGER(gfx);

        D3D11_BUFFER_DESC constBufDescr {};
        constBufDescr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constBufDescr.Usage = D3D11_USAGE_DYNAMIC;
        constBufDescr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constBufDescr.ByteWidth = sizeof(consts);
        constBufDescr.MiscFlags = {};
        constBufDescr.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA constBufSubSrcData {};
        constBufSubSrcData.pSysMem = &consts;

        GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&constBufDescr, &constBufSubSrcData, &constantBuffer_));

    }
    ConstantBuffer(Graphics& gfx) {
		INFOMANAGER( gfx );

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( C );
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO( gfx.getDevice()->CreateBuffer( &cbd,nullptr,&constantBuffer_) );
    }
    void Update(Graphics& gfx, const C& consts) {
        INFOMANAGER(gfx);

        D3D11_MAPPED_SUBRESOURCE mappedSubrsc {};
        GFX_THROW_INFO(
            gfx.getContext()->Map(constantBuffer_.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubrsc)
        );

        memcpy(mappedSubrsc.pData, &consts, sizeof(consts));
        gfx.getContext()->Unmap(constantBuffer_.Get(), 0u);
    }

    void Bind(Graphics& gfx) noexcept override {
        if constexpr (std::is_same<ShaderType, ID3D11VertexShader>()) {
            gfx.getContext()->VSSetConstantBuffers(0u, 1u, constantBuffer_.GetAddressOf());
        }else if constexpr (std::is_same<ShaderType, ID3D11PixelShader>()) {
            gfx.getContext()->PSSetConstantBuffers(0u, 1u, constantBuffer_.GetAddressOf());
        }
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer_;
};

template <class C>
class PixelConstantBuffer : public ConstantBuffer<C, ID3D11PixelShader> {
    using ConstantBuffer<C, ID3D11PixelShader>::ConstantBuffer;
};

template <class C>
class VertexConstantBuffer : public ConstantBuffer<C, ID3D11VertexShader> {
    using ConstantBuffer<C, ID3D11VertexShader>::ConstantBuffer;
};
