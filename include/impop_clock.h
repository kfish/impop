#pragma once

#include <ctime>
#include <cstdio>

#include "imgui.h"

namespace ImPop {

static inline void TextClock()
{
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    ImGui::Text("%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);
}

} // namespace ImPop
