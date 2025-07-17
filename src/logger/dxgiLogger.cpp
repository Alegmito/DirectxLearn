#include "dxgiLogger.h"
#include "../graphics/exceptions/graphicsException.h"
#include "../exceptions/windowException.h"
#include <Windows.h>
#include <dxgidebug.h>
#include <memory>

 #pragma comment(lib, "dxguid.lib")
 
 #define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw HResultException( __LINE__,__FILE__,hr )
 
DxgiInfo::DxgiInfo()
{
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID,void **);

    // load the dll that contains the function DXGIGetDebugInterface
    const auto dxgidebugLib { LoadLibraryEx( "dxgidebug.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32 )};
    if( dxgidebugLib == nullptr )
    {
        throw window_last_except();
    }

    // get address of DXGIGetDebugInterface in dll
    const auto DxgiGetDebugInterface { reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress( dxgidebugLib,"DXGIGetDebugInterface" ))
    )};
    if( DxgiGetDebugInterface == nullptr )
    {
        throw window_last_except();
    }

    (DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&dxgiInfoQueue_)));
}

DxgiInfo::~DxgiInfo()
{
    if( dxgiInfoQueue_ != nullptr )
    {
        dxgiInfoQueue_->Release();
    }
}

void DxgiInfo::set() noexcept
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    next_ = dxgiInfoQueue_->GetNumStoredMessages( DXGI_DEBUG_ALL );
}

std::vector<std::string> DxgiInfo::getLog() const {
    std::vector<std::string> messages {};
    const auto messagesEnd {dxgiInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL)};
    for (auto i {next_}; i < messagesEnd; ++i) {
        HRESULT hr {};
        SIZE_T msgLength {};
        GFX_THROW_NOINFO( dxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLength));
        auto msgBuffer = std::make_unique<uint8_t []>(msgLength);
        auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE *>(msgBuffer.get());

        GFX_THROW_NOINFO( dxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, message, &msgLength));
        messages.emplace_back(message->pDescription);
    }
    return messages;
}
