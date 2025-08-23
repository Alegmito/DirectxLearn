#include "Melon.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Sphere.h"
#include "Topology.h"
#include "TransformConstBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "box.h"
#include <DirectXMath.h>
#include <memory>
#include <array>
namespace dx = DirectX;

Melon::Melon(
    Graphics& gfx,
    std::mt19937& rng,
    std::uniform_real_distribution<float>& aDistrib,
    std::uniform_real_distribution<float>& dDistrib,
    std::uniform_real_distribution<float>& oDistrib,
    std::uniform_real_distribution<float>& rDistrib,
    std::uniform_int_distribution<int>& latDistrib,
    std::uniform_int_distribution<int>& longDistrib,
    std::shared_ptr<SharedBinds> binds
) : Drawable {binds},
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
    dtChi_ {oDistrib(rng)},
    latScale_ {latDistrib(rng)},
    longScale_ {longDistrib(rng)}
{

}

void Melon::Update(float deltaTime) noexcept {
    roll_ += dtRoll_ * deltaTime;
    pitch_ += dtPitch_ * deltaTime;
    yaw_ += dtYaw_ * deltaTime;
    theta_ += dtTheta_ * deltaTime;
    phi_ += dtPhi_ * deltaTime;
    chi_ += dtChi_ * deltaTime;
}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept {
    using namespace DirectX;
    return 
      XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_)
    * XMMatrixTranslation(-r_, 0, 0)
    * XMMatrixRotationRollPitchYaw(theta_, phi_, chi_)
    * XMMatrixTranslation(0, 0, 20)
    ;

}

void Melon::AddSharedBinds() {
    auto vs {std::make_unique<VertexShader>(gfx_, u"ColorIndexVS.cso")};
    auto vsCode {vs->getCodeBlob()};
    sharedBinds_->AddBind(std::move(vs));

    sharedBinds_->AddBind(std::make_unique<PixelShader>(gfx_, u"ColorIndexPS.cso"));


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


    const std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    sharedBinds_->AddBind(std::make_unique<InputLayout>(gfx_, vsCode, layoutDesc));

    sharedBinds_->AddBind(std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void Melon::AddLocalBinds() {
    auto model {Sphere::MakeTesselated<Vertex>(latScale_, longScale_)};
    model.Transform(dx::XMMatrixScaling(1.f, 1.f, 1.2f));

    AddBind(std::make_unique<VertexBuffer>(gfx_, model.vertices_));
    SetIndexBuffer(std::make_unique<IndexBuffer>(gfx_, model.indices_));
    AddBind(std::make_unique<TransformConstBuf>(gfx_, *this));
}
