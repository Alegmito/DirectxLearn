#pragma once
#include "directxException.h"
#include <Windows.h>
#include <winnt.h>

class WindowException : public DirectxException {
public:
    WindowException(HRESULT hr) noexcept;
    WindowException() noexcept;
    WindowException(int line, const char *fileName, HRESULT hr) noexcept;
    const char *what() const noexcept override;
    const char *getType() const noexcept override;
    static std::string TranslateErrorCode (HRESULT hr);
    HRESULT getErrorCode() const noexcept {return hr_;}
    std::string getErrorString() const noexcept;
private:
    HRESULT hr_;

};

#define window_except(hr) WindowException (__LINE__,__FILE__,(hr))
#define window_last_except() WindowException (__LINE__,__FILE__,GetLastError())

class NoGraphicsException : public DirectxException {
public:
    using DirectxException::DirectxException;
    const char *getType() const noexcept override;
};

#define no_graphics_exception() NoGraphicsException(__LINE__,__FILE__)
