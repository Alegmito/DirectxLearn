#include "VertexBuffer.h"

void VertexBuffer::Bind( Graphics& gfx ) noexcept {
    const UINT offsets {};
    gfx.getContext()->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride_, &offsets);
}


