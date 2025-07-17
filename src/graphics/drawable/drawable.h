#pragma once
#include "../graphics.h"
#include "../bindable/buffers/IndexBuffer.h"
#include "SharedBinds.hpp"
#include <DirectXMath.h>
#include <memory>

class IBindable;

class Drawable {
public:
    Drawable(std::shared_ptr<SharedBinds> sharedBinds = {}) : sharedBinds_(sharedBinds) {};
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    virtual void Update(float deltaTime) noexcept = 0;
    
    void Draw(Graphics &gfx) const;
    void AddBind(std::unique_ptr<IBindable> bind) noexcept;
    void SetIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;
    void SetIndexBuffer(IndexBuffer* buf) noexcept {indexBuffer_ = buf;} ;
    void Init();
protected:
    virtual void AddSharedBinds() = 0;
    virtual void AddLocalBinds() = 0;

private:
    const IndexBuffer* indexBuffer_ {};
    std::vector<std::unique_ptr<IBindable>> binds_ {};
protected:
    std::shared_ptr<SharedBinds> sharedBinds_ {};
};
