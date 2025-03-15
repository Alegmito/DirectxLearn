#pragma once
#include "directxException.h"
#include <Windows.h>
#include <winnt.h>

class WindowException : public DirectxException {
public:
    WindowException(int line, const char *fileName, HRESULT hr) noexcept;
    const char *what() const noexcept override;
    const char *getType() const noexcept override;
    static std::string TranslateErrorCode (HRESULT hr);
    HRESULT getErrorCode() const noexcept {return hr_;}
    std::string getErrorString() const noexcept;
private:
    HRESULT hr_;

};

#define Window_Except(hr) WindowException(__LINE__, __FILE__, hr);
