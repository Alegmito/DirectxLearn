#pragma once

#include <bitset>
#include <cstdint>
#include <optional>
#include <queue>
class KeyboardEvent {
public:
    enum class Type {
        press,
        release,
        invalid
    };
private:
    Type type_;
    uint8_t code_;
public:
    KeyboardEvent(Type type = Type::invalid, uint8_t code = 0)
        : type_ {type}, code_ {code}
    {}
    Type getType() noexcept {return type_;}
    uint8_t getCode () noexcept {return code_;}
};

// TODO: maybe create an interface for the class or the base class
class IKeyboardController { 
    virtual bool isKeyPressed(unsigned char keycode) const noexcept = 0;
    virtual std::optional<KeyboardEvent> readKey() noexcept = 0;
    virtual bool isKeyEmpty() const noexcept = 0;
    virtual void flushKey() noexcept = 0;

    virtual char ReadChar() noexcept = 0;
    virtual bool isEmptyChar() const noexcept = 0;
    virtual void flushChar() noexcept = 0;
    virtual void flush() noexcept = 0;

    virtual void enableAutorepeat() noexcept = 0;
    virtual void disableAutorepeat() noexcept = 0;
    virtual bool isAutorepeatEnabled() const noexcept = 0;
};

class KeyboardController {
public:
    KeyboardController() = default;
    KeyboardController &operator=(const KeyboardController &) = delete;

    bool isKeyPressed(unsigned char keycode) const noexcept;
    std::optional<KeyboardEvent> readKey() noexcept;
    bool isKeyEmpty() const noexcept;
    void clearKeyBuff() noexcept;

    char ReadChar() noexcept;
    bool isEmptyChar() const noexcept;
    void clearCharBuf() noexcept;
    void clearBuffers() noexcept;

    void setAutorepeat (bool isAutorepeat) {isAutorepeat_ = isAutorepeat;}
    bool isAutorepeat() const noexcept {return isAutorepeat_;}

    void onKeyPressed(unsigned char keycode) noexcept;
    void onKeyReleased(unsigned char keycode) noexcept;
    void onChar(int8_t ch) noexcept;
    void ClearState() noexcept;
    template<class T>
    static void trimBuffer(std::queue<T> &buffer) noexcept;
private:
    static constexpr unsigned int nKeys {256u};
    static constexpr unsigned int bufferSize {16u};
    bool isAutorepeat_ {false};
    std::bitset<nKeys> keystates_;
    std::queue<KeyboardEvent> keybuffer_;
    std::queue<int8_t> charbuffer_;
};
