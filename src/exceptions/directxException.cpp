#include "directxException.h"
#include <sstream>

DirectxException::DirectxException(int line, const char *fileName) noexcept
    : line_ {line}, file_ {fileName}
{}
const char *DirectxException::what() const noexcept {
    std::ostringstream oss {};
    oss << getType() << std::endl
        << getOriginString();
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}
const char *DirectxException::getType() const noexcept {
    return "DirectX exception";
}

std::string DirectxException::getOriginString() const noexcept {
    std::ostringstream oss {};
    oss << "[File] " << file_ << std::endl
        << "[Line] " << line_;
    return oss.str();
}
