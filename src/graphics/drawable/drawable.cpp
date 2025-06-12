#include "Drawable.h"

void Drawable::Draw(Graphics &gfx) const {
    for (auto& bind: binds_) {
        bind->Bind(gfx);
    }
    /*gfx.DrawIndexed(indexBuffer_->getCount());*/
}

void Drawable::AddBind(std::unique_ptr<IBindable> bind) noexcept{
    assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
    binds_.push_back(std::move(bind));
}

void Drawable::SetIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept{
    assert( "Attempting to add index buffer a second time" && indexBuffer_ == nullptr );
    indexBuffer_ = iBuf.get();
    binds_.push_back(std::move(iBuf));
}


