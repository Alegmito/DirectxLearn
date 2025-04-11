#include "../../exceptions/directxException.h"
#include <windows.h>

class GraphicsException : public DirectxException
{
    using DirectxException::DirectxException;
};

class HResultException: public DirectxException 
{
public:
    HResultException( int line,const char* file,HRESULT hr ) noexcept;
    const char* what() const noexcept override;
    const char* getType() const noexcept override;
    HRESULT getErrorCode() const noexcept;
    std::string getErrorString() const noexcept;
    std::string getErrorDescription() const noexcept;
private:
    HRESULT hr_;
};

class DeviceRemovedException : public HResultException
{
    using HResultException::HResultException;
public:
    const char* getType() const noexcept override;
};
