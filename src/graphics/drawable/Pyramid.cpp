#include "Pyramid.h"
#include "Box.h"
#include "PixelShader.h"
#include "TransformConstBuf.h"
#include "Cone.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include <memory>
#include <string>

Pyramid::Pyramid(
    Graphics& gfx,
    std::mt19937& rng,
    std::uniform_real_distribution<float>& aDistrib,
    std::uniform_real_distribution<float>& dDistrib,
    std::uniform_real_distribution<float>& oDistrib,
    std::uniform_real_distribution<float>& rDistrib,
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
    dtChi_ {oDistrib(rng)}
{

}

void Pyramid::Update(float deltaTime) noexcept {
    roll_ += dtRoll_ * deltaTime;
    pitch_ += dtPitch_ * deltaTime;
    yaw_ += dtYaw_ * deltaTime;
    theta_ += dtTheta_ * deltaTime;
    phi_ += dtPhi_ * deltaTime;
    chi_ += dtChi_ * deltaTime;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept {}

void Pyramid::AddSharedBinds() {
    namespace dx = DirectX;
    auto model {Cone::MakeTesselated<Vertex>(4)};
    model.vertices_[0].color = {255, 255, 0, 0.f};
    model.vertices_[1].color = {255, 255, 0, 0.f};
    model.vertices_[2].color = {255, 255, 0, 0.f};
    model.vertices_[3].color = {255, 255, 0, 0.f};
    model.vertices_[4].color = {255, 255, 80, 0.f};
    model.vertices_[5].color = {255, 10, 0, 0.f};
    model.Transform(dx::XMMatrixScaling(1.f, 1.f, 0.7f));

    sharedBinds_->AddBind(std::make_unique<VertexBuffer>(gfx_, model.vertices_));
    sharedBinds_->AddIndexBuf(std::make_unique<IndexBuffer>(gfx_, model.indices_));

    auto vertexShader {std::make_unique<VertexShader>(gfx_, u"ColorBlendVS.cso")};
    auto vertexShaderCode {vertexShader->getCodeBlob()};
    sharedBinds_->AddBind(std::move(vertexShader));

    sharedBinds_->AddBind(std::make_unique<PixelShader>(gfx_, u"ColorBlendPS.cso"));
}

void Pyramid::AddLocalBinds() {
    SetIndexBuffer(sharedBinds_->GetIndexBuf());
    AddBind(std::make_unique<TransformConstBuf>(gfx_, *this));
}
