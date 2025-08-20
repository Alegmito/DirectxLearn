#pragma once
#include "IBindable.h"
#include "../drawable/Box.h"
#include "buffers/ConstantBuffer.h"
#include <DirectXMath.h>

class TransformConstBuf : public IBindable {
public:
    TransformConstBuf(Graphics& gfx, Drawable& shape);
    void Bind(Graphics& gfx) noexcept override;
private:
    VertexConstantBuffer<DirectX::XMMATRIX> vertexConstBuf_;
    const Drawable& parent_;
};
