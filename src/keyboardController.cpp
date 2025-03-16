#include "keyboardController.h"
#include <cstdint>
#include <optional>

bool KeyboardController::isKeyPressed(unsigned char keycode) const noexcept {return keystates_[keycode];}

std::optional<KeyboardEvent> KeyboardController::readKey() noexcept {
    if (keybuffer_.size() > 0u) {
        KeyboardEvent event {keybuffer_.front()};
        keybuffer_.pop();
        return {event};
    }
    return std::nullopt;
}

bool KeyboardController::isKeyEmpty() const noexcept {return keybuffer_.empty();}

void KeyboardController::clearKeyBuff() noexcept {keybuffer_ = {};}

char KeyboardController::ReadChar() noexcept {
    if (charbuffer_.empty()) return 0;

    uint8_t charcode = charbuffer_.front();
    charbuffer_.pop();
    return charcode;
}

bool KeyboardController::isEmptyChar() const noexcept {return charbuffer_.empty();}

void KeyboardController::clearCharBuf() noexcept {charbuffer_ = {};}

void KeyboardController::clearBuffers() noexcept {clearKeyBuff(); clearCharBuf();}

void KeyboardController::onKeyPressed(uint8_t keycode) noexcept {
    keystates_[keycode] = true;
    keybuffer_.push(KeyboardEvent(KeyboardEvent::Type::press, keycode));
    trimBuffer(keybuffer_);
}

void KeyboardController::onKeyReleased(uint8_t keycode) noexcept {
    keystates_[keycode] = false;
    keybuffer_.push(KeyboardEvent(KeyboardEvent::Type::release, keycode));
    trimBuffer(keybuffer_);
}

void KeyboardController::onChar(int8_t ch) noexcept {
    charbuffer_.push(ch);
    trimBuffer(charbuffer_);
}

void KeyboardController::ClearState() noexcept {
    keystates_.reset();
}

template<typename T>
void KeyboardController::trimBuffer(std::queue<T> &buffer) noexcept {
    for (; buffer.size() > bufferSize; buffer.pop());
}
