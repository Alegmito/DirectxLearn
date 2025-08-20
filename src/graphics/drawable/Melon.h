#pragma once

#include "Drawable.h"
#include "graphics.h"
#include <random>

class Melon: public Drawable {
public:
    Melon( Graphics &graphics,
          std::mt19937 &rng,
          std::uniform_real_distribution<float>& aDistrib,
          std::uniform_real_distribution<float>& dDistrib,
          std::uniform_real_distribution<float>& oDistrib,
          std::uniform_real_distribution<float>& rDistrib,
          std::uniform_int_distribution<int>& latDistrib,
          std::uniform_int_distribution<int>& longDistrib,
          std::shared_ptr<SharedBinds> binds
    );
    void Update(float deltaTime) noexcept override;

    DirectX::XMMATRIX GetTransformXM() const noexcept override;
protected:
    void AddSharedBinds() override;
    void AddLocalBinds() override;

private:
    Graphics &gfx_;
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

    int latScale_ {};
    int longScale_ {};
};
