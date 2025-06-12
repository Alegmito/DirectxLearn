#pragma once
#include "IBindable.h"
#include <d3d11.h>

class Topology : public IBindable {
public:
    Topology(D3D11_PRIMITIVE_TOPOLOGY type);
    void Bind(Graphics& gfx) noexcept override;
protected:
    D3D11_PRIMITIVE_TOPOLOGY type_;
};
