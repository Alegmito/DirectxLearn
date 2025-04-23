#include "../../exceptions/directxException.h"
#include <vector>
#include <windows.h>

class GraphicsException : public DirectxException
{
    using DirectxException::DirectxException;
};

class HResultException: public GraphicsException 
{
public:
    HResultException( int line,const char* file,HRESULT hr, std::vector<std::string> msgs = {} ) noexcept;
    const char* what() const noexcept override;
    const char* getType() const noexcept override;
    HRESULT getErrorCode() const noexcept;
    std::string getErrorString() const noexcept;
    std::string getErrorDescription() const noexcept;
    std::string getInfo() const noexcept {return info_;}
private:
    HRESULT hr_;
    std::string info_ {};
};

class DeviceRemovedException : public HResultException
{
    using HResultException::HResultException;
public:
    const char* getType() const noexcept override;
private:
    std::string reason {};
};

class InfoException : public GraphicsException
{
public:
    InfoException( int line,const char* file,std::vector<std::string> infoMsgs ) noexcept;
    const char* what() const noexcept override;
    const char* getType() const noexcept override;
    std::string getInfo() const noexcept;
private:
    std::string info_;
};
