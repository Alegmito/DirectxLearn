#include "Topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
    : type_(type)
{ }
void Topology::Bind(Graphics& gfx) noexcept {
    gfx.getContext()->IASetPrimitiveTopology(type_);
}
