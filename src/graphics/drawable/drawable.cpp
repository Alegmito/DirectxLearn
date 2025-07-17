#include "Drawable.h"
#include <algorithm>
#include <mutex>

void Drawable::Draw(Graphics &gfx) const {
    std::for_each(binds_.begin(), binds_.end(), [&] (auto& bind) {bind->Bind(gfx);});
    std::for_each(sharedBinds_->binds_.begin(), sharedBinds_->binds_.end(), [&] (auto& bind) {bind->Bind(gfx);});

    gfx.DrawIndexed(indexBuffer_->GetCount());
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


void Drawable::Init() {
    {
        auto lock {std::lock_guard<std::mutex> {sharedBinds_->GetMutex()}};

        if (!sharedBinds_->IsInitialized()) {
            AddSharedBinds();
            sharedBinds_->SetIsInitialized(true);
        }
    }
    AddLocalBinds();
}
