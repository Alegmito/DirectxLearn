#include "windowException.h"
#include "directxException.h"
#include <sstream>
#include <string_view>
#include <winbase.h>
#include <winnt.h>

WindowException::WindowException(int line, const char *fileName, HRESULT hr) noexcept
    : DirectxException(line, fileName)
    , hr_(hr)
{}
constexpr std::string_view window_exception_type {"DirectX Window Exception"};

const char *WindowException::what() const noexcept {
    std::ostringstream oss {};
    oss << getType() << std::endl
        << "[Error Code]" << getErrorCode() << std::endl
        << "[Description]" << getErrorString() << std::endl
        << getOriginString();
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}


const char *WindowException::getType() const noexcept {return window_exception_type.data();}

std::string WindowException::TranslateErrorCode (HRESULT hr) {
    char *pMsgBuff {nullptr};
    auto msgLength { FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuff), 0, nullptr
    )};
    if (0 == msgLength) {
        return "Unedentified error code";
    }
    std::string errorStr {pMsgBuff};
    LocalFree(pMsgBuff);
    return errorStr.c_str();
}

std::string WindowException::getErrorString() const noexcept {
    return TranslateErrorCode(hr_);
}
