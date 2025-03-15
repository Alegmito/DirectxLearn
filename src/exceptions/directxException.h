#pragma once
#include <exception>
#include <string>

class DirectxException : public std::exception {
public:
    DirectxException(int line, const char *fileName) noexcept;
    const char *what() const noexcept override;
    virtual const char *getType() const noexcept;
    int getCodeLine() const noexcept {return line_;}
    const std::string &getFile() const noexcept {return file_;}
    std::string getOriginString() const noexcept;
private:
    int line_;
    std::string file_;
protected:
    mutable std::string whatBuffer_ {};
};
