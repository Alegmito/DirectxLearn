#pragma once
#include "../IBindable.h"
#include "../../exceptions/graphicsExMacros.h"
#include <vector>

class VertexBuffer : public IBindable {
public:
    template <class V>
    VertexBuffer(Graphics &gfx, const std::vector<V>& vertices)
    : stride_(sizeof(V)) {
        INFOMANAGER(gfx);
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer {};
        auto buffDescr {D3D11_BUFFER_DESC {}};
        buffDescr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffDescr.Usage = D3D11_USAGE_DEFAULT;
        buffDescr.CPUAccessFlags = {};
        buffDescr.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(V));
        buffDescr.MiscFlags = {};
        buffDescr.StructureByteStride = sizeof(V);

        auto subrscData {D3D11_SUBRESOURCE_DATA {}};
        subrscData.pSysMem = vertices.data();

        GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&buffDescr, &subrscData, &vertexBuffer));
    }
	void Bind( Graphics& gfx ) noexcept override;

protected:
    UINT stride_ {};
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
};
