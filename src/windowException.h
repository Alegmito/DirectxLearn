#pragma once
#include "directxException.h"
#include <Windows.h>

class WindowException : public DirectxException {
public:
    WindowException(int line, const char *fileName) noexcept;
    const char *what() const noexcept override;
    const char *getType() const noexcept override;
private:
    HRESULT hr;

};
