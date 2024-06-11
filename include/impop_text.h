#pragma once

#include "imgui.h"

namespace ImPop {

static inline void OutlineTextColoredV(ImDrawList* drawList, const ImVec2& pos,
                                       ImU32 text_color, ImU32 outline_color,
                                       const char* fmt, va_list args)
{
    static char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // Draw the outline (four directions around the original position)
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), outline_color, buffer);
    drawList->AddText(ImVec2(pos.x - 1, pos.y + 1), outline_color, buffer);
    drawList->AddText(ImVec2(pos.x + 1, pos.y - 1), outline_color, buffer);
    drawList->AddText(ImVec2(pos.x - 1, pos.y - 1), outline_color, buffer);

    // Draw the main text
    drawList->AddText(pos, text_color, buffer);
}

static inline void OutlineTextV(ImDrawList* drawList, const ImVec2& pos,
                                const char* fmt, va_list args)
{
    OutlineTextColoredV(drawList, pos, IM_COL32_WHITE, IM_COL32_BLACK, fmt, args);
}

static inline void OutlineTextColored(ImDrawList* drawList, const ImVec2& pos,
                                      ImU32 text_color, ImU32 outline_color,
                                      const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    OutlineTextColoredV(drawList, pos, text_color, outline_color, fmt, args);
    va_end(args);
}

static inline void OutlineText(ImDrawList* drawList, const ImVec2& pos, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    OutlineTextColoredV(drawList, pos, IM_COL32_WHITE, IM_COL32_BLACK, fmt, args);
    va_end(args);
}


} // namespace ImPop
