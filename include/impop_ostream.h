#pragma once

#include <ostream>

#include "imgui.h"

static inline std::ostream& operator<<(std::ostream& os, const ImVec2& v) {
    return os << "(" << v.x << "," << v.y << ")";
}

static inline std::ostream& operator<<(std::ostream& os, const ImVec4& v) {
    return os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
}
