#include "box.h"
#include <DirectXMath.h>
#include <array>
#include <memory>
#include <utility>
#include "Cube.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformConstBuf.h"
#include "Drawable.h"

using namespace std;
namespace dx = DirectX;


Box::Box(
    Graphics& gfx,
    mt19937& rng,
    uniform_real_distribution<float>& aDistrib,
    uniform_real_distribution<float>& dDistrib,
    uniform_real_distribution<float>& oDistrib,
    uniform_real_distribution<float>& rDistrib,
    uniform_real_distribution<float>& bDistrib,
    shared_ptr<SharedBinds> binds
) :
    Drawable(binds),
    gfx_ {gfx},
    r_ {rDistrib(rng)},
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
    dx::XMStoreFloat3x3(&transform_, dx::XMMatrixScaling(1.f, 1.f, bDistrib(rng)));
}

void Box::AddSharedBinds() {
    auto cube_model {Cube::Make<VertexPos>()};
    sharedBinds_->AddBind(make_unique<VertexBuffer>(gfx_, cube_model.vertices_));
    sharedBinds_->AddIndexBuf(make_unique<IndexBuffer>(gfx_, cube_model.indices_));

    auto vShader {make_unique<VertexShader>(gfx_, u16string{u"ColorIndexVS.cso"})};
    ID3DBlob* vsBlob {vShader->getCodeBlob()};
    sharedBinds_->AddBind(std::move(vShader));

    auto pShader {make_unique<PixelShader>(gfx_, u16string{u"ColorIndexPS.cso"})};
    sharedBinds_->AddBind(std::move(pShader));

    const std::array<Color, 8> colorBuffer {{
        {1.f, 1.f, 1.f, 0.f},
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {1.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {1.f, 0.f, 1.f, 0.f},
        {0.f, 1.f, 1.f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
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
      XMLoadFloat3x3(&transform_)
    * XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_)
    * XMMatrixTranslation(-r_, 0, 0)
    * XMMatrixRotationRollPitchYaw(theta_, phi_, chi_)
    * XMMatrixTranslation(0, 0, 20)
    ;
}
