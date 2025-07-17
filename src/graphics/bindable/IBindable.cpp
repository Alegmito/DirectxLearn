#include "../exceptions/graphicsExMacros.h"
#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, ID3DBlob* blob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout) {
    INFOMANAGER(gfx);

    GFX_THROW_INFO( gfx.getDevice()->CreateInputLayout(
        layout.data(), layout.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout_)
    );
}

void InputLayout::Bind(Graphics& gfx) {
    gfx.getContext()->IASetInputLayout(inputLayout_.Get());
}
