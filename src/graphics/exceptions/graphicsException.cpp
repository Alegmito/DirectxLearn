#include "graphicsException.h"
#include "dxerr.h"
#include <cstdint>
#include <ios>
#include <oleauto.h>
#include <sstream>

HResultException::HResultException( int line,const char* file,HRESULT hr, std::vector<std::string> msgs) noexcept
    : GraphicsException{line, file}, hr_ {hr}
{
    for (auto &m : msgs) {
        info_ += m;
        info_ += '\n';
    }

    if(!info_.empty()) { info_.pop_back(); }
}

const char* HResultException::what() const noexcept  {
    std::ostringstream oss {};
    oss << getType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << static_cast<uint32_t>(getErrorCode()) << ")" << std::endl
        << "[Error String] " << getErrorString() << std::endl
        << "[Description] " << getErrorDescription() << std::endl;

    if (!info_.empty()) {
        oss << "\n[Error Info]\n" << getInfo() << std::endl << std::endl;
    }

    oss << getOriginString();

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
    return "Directx Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

InfoException::InfoException( int line,const char * file,std::vector<std::string> infoMsgs ) noexcept
 	:
 	GraphicsException( line,file )
 {
 	// join all info messages with newlines into single string
 	for( const auto& m : infoMsgs )
 	{
 		info_ += m;
 		info_.push_back( '\n' );
 	}
 	// remove final newline if exists
 	if( !info_.empty() )
 	{
 		info_.pop_back();
 	}
 }
 
 
 const char* InfoException::what() const noexcept
 {
 	std::ostringstream oss;
 	oss << getType() << std::endl
 		<< "\n[Error Info]\n" << getInfo() << std::endl << std::endl;
 	oss << getOriginString();
 	whatBuffer_ = oss.str();
 	return whatBuffer_.c_str();
 }
 
 const char* InfoException::getType() const noexcept
 {
 	return "Directx Graphics Info Exception";
 }
 
 std::string InfoException::getInfo() const noexcept
 {
 	return info_;
 }
