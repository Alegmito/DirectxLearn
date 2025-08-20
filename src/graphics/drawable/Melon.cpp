#include "Melon.h"
#include <DirectXMath.h>
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

void Melon::Update(float deltaTime) noexcept {}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept {
    return dx::XMMatrixRotationRollPitchYaw(roll_, pitch_, yaw_);
}

void Melon::AddSharedBinds() {

}

void Melon::AddLocalBinds() {
}
