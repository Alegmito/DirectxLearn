#include "PixelShader.h"
#include "../../exceptions/graphicsExMacros.h"
#include "../../exceptions/graphicsException.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& gfx, std::u16string_view path) {
    INFOMANAGER(gfx);
    const auto wide {std::u16string{path.begin(), path.end()}};
    GFX_THROW_INFO( D3DReadFileToBlob( reinterpret_cast<const LPCWSTR>(wide.c_str()), &blob_));
    GFX_THROW_INFO(
        gfx.getDevice()->CreatePixelShader(blob_->GetBufferPointer(), blob_->GetBufferSize(), nullptr, &pixelShader_)
    );
}

void PixelShader::Bind (Graphics &gfx) {
    gfx.getContext()->PSSetShader(pixelShader_.Get(), nullptr, 0u);
}
