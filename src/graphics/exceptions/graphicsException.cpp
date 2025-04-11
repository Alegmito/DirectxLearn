#include "graphicsException.h"
#include "dxerr.h"
#include <cstdint>
#include <ios>
#include <sstream>

HResultException::HResultException( int line,const char* file,HRESULT hr ) noexcept
    : DirectxException{line, file}, hr_ {hr}
{}

const char* HResultException::what() const noexcept  {
    std::ostringstream oss {};
    oss << getType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << static_cast<uint32_t>(getErrorCode()) << ")" << std::endl
        << "[Error String] " << getErrorString() << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

const char* HResultException::getType() const noexcept  {
    return "Directx Graphics Exception";
}

HRESULT HResultException::getErrorCode() const noexcept {
    return hr_;
}

std::string HResultException::getErrorString() const noexcept {
    return DXGetErrorString(hr_);
}

constexpr uint32_t errorDescBufSize {512};
std::string HResultException::getErrorDescription() const noexcept {
    char buf[errorDescBufSize];
    DXGetErrorDescription(hr_, buf, sizeof(buf));
    return buf;
}

const char* DeviceRemovedException::getType() const noexcept {
    return "Directx Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED}";
}
