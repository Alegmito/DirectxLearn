#pragma once
#include <utility>

struct MouseState {
    int x;
    int y;
    bool isLeftPressed;
    bool isRightPressed;
    std::pair<int, int> getPos() const {return {x, y};}
};

class MouseEvent {
public:
    enum class Type {
        lPress,
        lRelease,
        rPress,
        rRelease,
        wheelUp,
        wheelDown,
        move,
        enter,
        leave,
        invalid
    };

    MouseEvent(Type type,const MouseState &state) noexcept
        : type_ {type},
        state_ {state}
    {}
    MouseState &getState() noexcept {return state_;}
    Type getType() const {return type_;}
private:
    Type type_;
    MouseState state_;
};
