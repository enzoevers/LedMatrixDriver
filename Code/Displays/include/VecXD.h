#pragma once

#include <stdint.h>

// TODO: Make this templated for the type of the x and y
struct Vec2D {
    uint32_t x = 0;
    uint32_t y = 0;

    auto operator==(const Vec2D& rhs) const -> bool { return (this->x == rhs.x && this->y == rhs.y); }
    auto operator!=(const Vec2D& rhs) const -> bool { return (this->x != rhs.x || this->y != rhs.y); }
};