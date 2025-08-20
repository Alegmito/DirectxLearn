#pragma  once
#include "../bindable/IBindable.h"
#include "../bindable/buffers/IndexBuffer.h"
#include <memory>
#include <mutex>
#include <vector>

struct SharedBinds {
    SharedBinds() {};
    SharedBinds(std::shared_ptr<std::mutex> mutex) : mutex_ {mutex} {};

    std::vector<std::unique_ptr<IBindable>> binds_ {};

    void AddBind(std::unique_ptr<IBindable> bind) {
    // static void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept(!IS_DEBUG)
        assert( "*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
		binds_.push_back( std::move( bind ) );
    }

    void AddIndexBuf(std::unique_ptr<IndexBuffer> bind) {
		assert( "Attempting to add index buffer a second time" && ibuf_ == nullptr );
		ibuf_ = bind.get();
		binds_.push_back( std::move(bind) );
    }

    void SetMutex(std::shared_ptr<std::mutex> mutex) {mutex_ = mutex;}
    std::mutex& GetMutex() {return *mutex_.get();}
    IndexBuffer * GetIndexBuf() {return ibuf_;}
    bool IsInitialized() {return isInitialized_;}
    void SetIsInitialized(bool isInitialized) {isInitialized_ = isInitialized;}
private:
    std::shared_ptr<std::mutex> mutex_ {};
    IndexBuffer* ibuf_ {};
    bool isInitialized_ {};
};
