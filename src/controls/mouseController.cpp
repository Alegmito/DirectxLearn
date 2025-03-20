#include "mouseController.h"
#include <optional>
#include <Windows.h>

std::optional<MouseEvent> MouseController::Read() noexcept {
    if (buffer_.size() > 0) {
        MouseEvent e = buffer_.front();
        buffer_.pop();
        return e;
    }
    return std::nullopt;
}

void MouseController::onMouseMove(int x, int y) noexcept {
    state_.x = x;
    state_.y = y;

    buffer_.push({MouseEvent(MouseEvent::Type::move, getState())});
    trimBuffer();
}

void MouseController::onLeftPressed(int, int) noexcept {
    state_.isLeftPressed = true;

    buffer_.push({MouseEvent(MouseEvent::Type::lPress, getState())});
    trimBuffer();
}

void MouseController::onLeftReleased(int, int) noexcept {
    state_.isLeftPressed = false;

    buffer_.push({MouseEvent(MouseEvent::Type::lRelease, getState())});
    trimBuffer();
}

void MouseController::onRightPressed(int, int) noexcept {
    state_.isRightPressed = true;

    buffer_.push({MouseEvent(MouseEvent::Type::rPress, getState())});
    trimBuffer();
}

void MouseController::onRightReleased(int, int) noexcept {
    state_.isRightPressed = false;

    buffer_.push({MouseEvent(MouseEvent::Type::rRelease, getState())});
    trimBuffer();
}

void MouseController::onWheelUp(int, int) noexcept {
    buffer_.push({MouseEvent(MouseEvent::Type::wheelUp, getState())});
    trimBuffer();
}

void MouseController::onWheelDown(int, int) noexcept {
    buffer_.push({MouseEvent(MouseEvent::Type::wheelDown, getState())});
    trimBuffer();
}

// TODO: maybe move queue trim to the utiliint, intties
void MouseController::trimBuffer() noexcept {
    for (; buffer_.size() > bufferSize; buffer_.pop());
}

void MouseController::onMouseLeave() noexcept {
    isInWindow_ = false;
    buffer_.push({MouseEvent(MouseEvent::Type::leave, getState())});
    trimBuffer();
}

void MouseController::onMouseEnter() noexcept {
    isInWindow_ = true;
    buffer_.push({MouseEvent(MouseEvent::Type::enter, getState())});
    trimBuffer();
}

void MouseController::onWheelDelta(int x, int y, int delta) noexcept {
    wheelDeltaCarry_ += delta;
    while(wheelDeltaCarry_ >= WHEEL_DELTA) {
        wheelDeltaCarry_ -= WHEEL_DELTA;
        onWheelUp(x, y);
    }
    while(wheelDeltaCarry_ >= -WHEEL_DELTA) {
        wheelDeltaCarry_ += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}
