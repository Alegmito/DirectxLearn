#pragma once
#include "../graphics.h"
#include "../bindable/buffers/IndexBuffer.h"
#include <DirectXMath.h>
#include <memory>

class IBindable;

class Drawable {
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    virtual void Update(float deltaTime) noexcept = 0;
    
    void Draw(Graphics &gfx) const;
    void AddBind(std::unique_ptr<IBindable> bind) noexcept;
    void SetIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;

private:
    const IndexBuffer* indexBuffer_ {};
    std::vector<std::unique_ptr<IBindable>> binds_ {};
};
