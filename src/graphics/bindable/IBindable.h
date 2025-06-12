#pragma once
#include "../graphics.h"

class IBindable {
public:
    virtual void Bind (Graphics &gfx) = 0;
    virtual ~IBindable() = default;
};
