#pragma once
#include "Drawable.h"
#include "SharedBinds.hpp"
#include <DirectXMath.h>
#include <random>

class Box : public Drawable {
public:
    Box(
        Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& aDistrib,
        std::uniform_real_distribution<float>& dDistrib,
        std::uniform_real_distribution<float>& oDistrib,
        std::uniform_real_distribution<float>& rDistrib,
        std::uniform_real_distribution<float>& bDistrib,
        std::shared_ptr<SharedBinds> binds
    );
    void Update(float deltaTime) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
protected:
    void AddSharedBinds() override;
    void AddLocalBinds() override;

private:
    Graphics& gfx_;
    float r_ {};
    // rotation around Object Center
    float roll_ {};
    float pitch_ {};
    float yaw_ {};
    //rotation around world
    float theta_ {};
    float phi_ {};
    float chi_ {};
    // speed (delta/s)
    float dtRoll_ {};
    float dtPitch_ {};
    float dtYaw_ {};
    float dtTheta_ {};
    float dtPhi_ {};
    float dtChi_ {};
    DirectX::XMFLOAT3X3 transform_ {};
};

struct Position { float x; float y; float z; };
struct Color { float r; float g; float b; float a; };
struct UColor { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
struct Vertex { DirectX::XMFLOAT3 pos; UColor color; };
struct VertexPos {DirectX::XMFLOAT3 pos;};
