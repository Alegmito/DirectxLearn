#include "Box.h"
#include <DirectXMath.h>
#include <array>
#include <memory>
#include <utility>
#include "../bindable/buffers/VertexBuffer.h"
#include "../bindable/buffers/ConstantBuffer.h"
#include "../bindable/shaders/VertexShader.h"
#include "../bindable/shaders/PixelShader.h"
#include "../bindable/InputLayout.h"
#include "../bindable/Topology.h"
#include "../bindable/TransformConstBuf.h"
#include "Drawable.h"

using namespace std;


Box::Box(
    Graphics& gfx,
    mt19937& rng,
    uniform_real_distribution<float>& aDistrib,
    uniform_real_distribution<float>& dDistrib,
    uniform_real_distribution<float>& oDistrib,
    uniform_real_distribution<float>& rDistrib,
    std::shared_ptr<SharedBinds> binds
) :
    Drawable(binds),
    gfx_ {gfx},
    r {rDistrib(rng)},
    theta_ {aDistrib(rng)},
    phi_ {aDistrib(rng)},
    chi_ {aDistrib(rng)},
    dtRoll_ {dDistrib(rng)},
    dtPitch_ {dDistrib(rng)},
    dtYaw_ {dDistrib(rng)},
    dtTheta_ {oDistrib(rng)},
    dtPhi_ {oDistrib(rng)},
    dtChi_ {oDistrib(rng)}
{
}

void Box::AddSharedBinds() {
    vector<Position> vertices {{
        {-1.f, -1.f, -1.f},
        {1.f, -1.f, -1.f},
        {-1.f, 1.f, -1.f},
        {1.f, 1.f, -1.f},
        {-1.f, -1.f, 1.f},
        {1.f, -1.f, 1.f},
        {-1.f, 1.f, 1.f},
        {1.f, 1.f, 1.f}
    }};

    sharedBinds_->AddBind(make_unique<VertexBuffer>(gfx_, vertices));

    auto vShader {make_unique<VertexShader>(gfx_, u16string{u"vertexShader.cso"})};
    ID3DBlob* vsBlob {vShader->getCodeBlob()};
    sharedBinds_->AddBind(std::move(vShader));

    auto pShader {make_unique<PixelShader>(gfx_, u16string{u"pixelShader.cso"})};
    sharedBinds_->AddBind(std::move(pShader));

    vector<uint16_t> indices {
        0, 2, 1,  2, 3, 1,
        1, 3, 5,  3, 7, 5,
        2, 6, 3,  3, 6, 7,
        4, 5, 7,  4, 7, 6,
        0, 4, 2,  2, 4, 6,
        0, 1, 4,  1, 5, 4
    };
    sharedBinds_->AddIndexBuf(make_unique<IndexBuffer>(gfx_, indices));

    const std::array<Color, 6> colorBuffer {{
        {1.f, 0.f, 1.f, 0.f},
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {1.f, 1.f, 0.f, 0.f},
        {0.f, 1.f, 1.f, 0.f},
    }};
    sharedBinds_->AddBind(make_unique<PixelConstantBuffer<typeof(colorBuffer)>>(gfx_, colorBuffer));

    const vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    sharedBinds_->AddBind(make_unique<InputLayout>(gfx_, vsBlob, layoutDesc));

    sharedBinds_->AddBind(make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void Box::AddLocalBinds() {
    SetIndexBuffer(sharedBinds_->GetIndexBuf());
    AddBind(make_unique<TransformConstBuf>(gfx_, *this));
}

void Box::Update(float deltaTime) noexcept {
    roll_ += dtRoll_ * deltaTime;
    pitch_ += dtPitch_ * deltaTime;
    yaw_ += dtYaw_ * deltaTime;
    theta_ += dtTheta_ * deltaTime;
    phi_ += dtPhi_ * deltaTime;
    chi_ += dtChi_ * deltaTime;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept {
    using namespace DirectX;
    return 
      XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_)
    * XMMatrixTranslation(-r, 0, 0)
    * XMMatrixRotationRollPitchYaw(theta_, phi_, chi_)
    * XMMatrixTranslation(0, 0, 20)
    ;
}
