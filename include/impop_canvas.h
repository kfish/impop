#pragma once

#include "imgui.h"

#include "impop_text.h"

namespace ImPop {

struct Canvas {
    ImVec2 top_left;
    ImDrawList* draw_list;

    Canvas()
        : top_left(ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin()),
        draw_list(ImGui::GetWindowDrawList())
    {}

    void Line(const ImVec2& start, const ImVec2& end, const ImU32 color) {
        ImVec2 lmin = top_left + start;
        ImVec2 lmax = top_left + end;
        draw_list->AddLine(lmin, lmax, color);
    }

    void Rect(const ImVec2& rect_min, const ImVec2& rect_dim, const ImU32 color) {
        ImVec2 rmin = top_left + rect_min;
        ImVec2 rmax = rmin + rect_dim;
        draw_list->AddRect(rmin, rmax, color);
    }

    void RectFilled(const ImVec2& rect_min, const ImVec2& rect_dim, const ImU32 color) {
        ImVec2 rmin = top_left + rect_min;
        ImVec2 rmax = rmin + rect_dim;
        draw_list->AddRect(rmin, rmax, color);
    }

    void RectFilledBorder(const ImVec2& rect_min, const ImVec2& rect_dim,
                          const ImU32 border_color, const ImU32 fill_color) {
        ImVec2 rmin = top_left + rect_min;
        ImVec2 rmax = rmin + rect_dim;
        draw_list->AddRectFilled(rmin, rmax, fill_color);
        draw_list->AddRect(rmin, rmax, border_color);
    }

    void TextClipRect(const ImVec2& clip_min, const ImVec2& clip_dim, const ImVec2& text_offset,
                   const char * text_str, const ImU32 text_color) {
        ImVec2 cmin = top_left + clip_min;
        ImVec2 cmax = cmin + clip_dim;
        draw_list->PushClipRect(cmin, cmax, true);
        draw_list->AddText(cmin+text_offset, text_color, text_str);
        draw_list->PopClipRect();
    }

    void OutlineText(const ImVec2& pos, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ImPop::OutlineTextV(draw_list, top_left + pos, fmt, args);
        va_end(args);
    }
};

} // namespace ImPop
