#include "VertexShader.h"
#include "../../exceptions/graphicsExMacros.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& gfx, std::u16string_view path) {
    INFOMANAGER(gfx);
    GFX_THROW_INFO( D3DReadFileToBlob( reinterpret_cast<const LPCWSTR>(path.data()), &blob_));
    GFX_THROW_INFO(
        gfx.getDevice()->CreateVertexShader(blob_->GetBufferPointer(), blob_->GetBufferSize(), nullptr, &vertexShader_)
    );
}

void VertexShader::Bind (Graphics &gfx) {
    gfx.getContext()->VSSetShader(vertexShader_.Get(), nullptr, 0u);
}
