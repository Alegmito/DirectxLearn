#include "IndexBuffer.h"
#include "../../exceptions/graphicsExMacros.h"
#include "../../exceptions/graphicsException.h"
#include <dxgiformat.h>

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<uint32_t>& indices) 
    : count_(indices.size())
{
    INFOMANAGER(gfx);
    auto iBuffDescr {D3D11_BUFFER_DESC {}};
    iBuffDescr.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBuffDescr.Usage = D3D11_USAGE_DEFAULT;
    iBuffDescr.CPUAccessFlags = {};
    iBuffDescr.ByteWidth = static_cast<UINT>(count_ * sizeof(uint32_t));
    iBuffDescr.MiscFlags = {};
    iBuffDescr.StructureByteStride = sizeof(uint32_t);

    auto iSubrscData {D3D11_SUBRESOURCE_DATA {}};
    iSubrscData.pSysMem = indices.data();

    GFX_THROW_INFO(gfx.getDevice()->CreateBuffer(&iBuffDescr, &iSubrscData, &indexBuffer_));
};

void IndexBuffer::Bind(Graphics& gfx) noexcept {
    gfx.getContext()->IASetIndexBuffer(indexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0u);
}
