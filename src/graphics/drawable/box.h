#pragma once
#include "Drawable.h"
#include <random>

class Box : public Drawable {
public:
    Box(
        Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& aDistrib,
        std::uniform_real_distribution<float>& dDistrib,
        std::uniform_real_distribution<float>& oDistrib,
        std::uniform_real_distribution<float>& rDistrib
    );
    void Update(float deltaTime) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
    float r {};
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

};

struct Position { float x; float y; float z; };
struct Color { float r; float g; float b; float a; };
struct Vertex { Position v; Color color; };
