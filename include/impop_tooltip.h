#pragma once

#include "imgui.h"

namespace ImPop {

// Adapted from imgui_demo.cpp
void Tooltip(const char* fmt, ...)
{
    static char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);

    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(buf);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

} // namespace ImPop
