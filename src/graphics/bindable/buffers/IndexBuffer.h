#pragma once
#include "../IBindable.h"
#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h>

struct IndexBuffer : public IBindable {
    IndexBuffer(Graphics& gfx, const std::vector<uint32_t>& indices);
    void Bind(Graphics& gfx) noexcept override;
    UINT GetCount() const noexcept {return count_;}
protected:
    UINT count_ {};
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_ {};
};
