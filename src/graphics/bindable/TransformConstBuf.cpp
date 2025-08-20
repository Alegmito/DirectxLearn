#include "TransformConstBuf.h"
#include <DirectXMath.h>

TransformConstBuf::TransformConstBuf(Graphics& gfx, Drawable& shape)
    : vertexConstBuf_{gfx}, parent_(shape)
{
}

void TransformConstBuf::Bind(Graphics &gfx) noexcept{
    using namespace DirectX;
    vertexConstBuf_.Update(gfx, XMMatrixTranspose(parent_.GetTransformXM() * gfx.GetProjection()));
    vertexConstBuf_.Bind(gfx);
}
