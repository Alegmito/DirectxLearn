#pragma once

#include <string>
#include <vector>

class DxgiInfo {
public:
    DxgiInfo();
    ~DxgiInfo();
    
    DxgiInfo(const DxgiInfo &) = delete;
    DxgiInfo &operator=(const DxgiInfo &) = delete;
    void set() noexcept;
    std::vector<std::string> getLog() const;
private:
 	unsigned long long next_ = 0u;
 	struct IDXGIInfoQueue* dxgiInfoQueue_ = nullptr;
};
