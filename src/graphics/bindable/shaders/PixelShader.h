#pragma once 
#include "../IBindable.h"

class PixelShader : public IBindable {
public:
    PixelShader(Graphics& gfx, std::u16string_view path);
    virtual void Bind (Graphics &gfx);
    auto getCodeBlob() {return blob_.Get();}

private:
    Microsoft::WRL::ComPtr<ID3DBlob> blob_ {};
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_ {};
};
