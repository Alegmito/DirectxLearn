#pragma once

#include "IBindable.h"
#include <d3d11.h>
class InputLayout : public IBindable {
public:
    InputLayout(Graphics& gfx, ID3DBlob* blob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);

    void Bind(Graphics& gfx) override;
private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_ {};
};
