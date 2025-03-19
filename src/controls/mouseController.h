#include <optional>
#include <queue>
#include "mouseEvent.h"


class MouseController {
public:
    MouseController() = default;
    MouseController(const MouseController &) = delete;
    MouseController &operator=(const MouseController &) = delete;
    std::optional<MouseEvent> Read() noexcept;
    bool isEmpty() const noexcept {return buffer_.empty();}
    void clearBuffer() noexcept {buffer_ = {};}

    void onMouseMove(int x, int y) noexcept;
    void onLeftPressed(int x, int y) noexcept;
    void onLeftReleased(int x, int y) noexcept;
    void onRightPressed(int x, int y) noexcept;
    void onRightReleased(int x, int y) noexcept;
    void onWheelUp(int x, int y) noexcept;
    void onWheelDown(int x, int y) noexcept;
    void onWheelDelta(int x, int y, int delta) noexcept;
    void trimBuffer() noexcept;
    void onMouseLeave() noexcept;
    void onMouseEnter() noexcept;
    bool isInWindow() noexcept {return isInWindow_;}

    MouseState &getState() noexcept {return state_;}
private:
    static constexpr unsigned int bufferSize {16};
    MouseState state_ {};
    std::queue<MouseEvent> buffer_;
    bool isInWindow_ {false};
    int wheelDeltaCarry_ {};
};
