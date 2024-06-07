#pragma once

#include "imgui.h"

namespace ImPop {

static inline void OutlineTextV(ImDrawList* drawList, const ImVec2& pos, const char* fmt, va_list args) {
    // Buffer to hold the formatted text
    static char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // Draw the outline (four directions around the original position)
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32_BLACK, buffer);
    drawList->AddText(ImVec2(pos.x - 1, pos.y + 1), IM_COL32_BLACK, buffer);
    drawList->AddText(ImVec2(pos.x + 1, pos.y - 1), IM_COL32_BLACK, buffer);
    drawList->AddText(ImVec2(pos.x - 1, pos.y - 1), IM_COL32_BLACK, buffer);

    // Draw the main text
    drawList->AddText(pos, IM_COL32_WHITE, buffer);
}

static inline void OutlineText(ImDrawList* drawList, const ImVec2& pos, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    OutlineTextV(drawList, pos, fmt, args);
    va_end(args);
}

} // namespace ImPop
