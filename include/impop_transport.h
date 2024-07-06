#pragma once

#include "imgui.h"

namespace ImPop {

static inline void PlayIcon(ImDrawList* drawList, const ImVec2& rect_min, const ImVec2& rect_dim, ImU32 color)
{
    ImVec2 p2 = ImVec2(rect_min.x + rect_dim.x, rect_min.y + rect_dim.y/2.0);
    ImVec2 p3 = ImVec2(rect_min.x, rect_min.y + rect_dim.y);
    drawList->AddTriangleFilled(rect_min, p2, p3, color);
}

static inline void PauseIcon(ImDrawList* drawList, const ImVec2& rect_min, const ImVec2& rect_dim, ImU32 color)
{
    ImVec2 rect_max = ImVec2(rect_min.x + rect_dim.x, rect_min.y + rect_dim.y);
    {
        ImVec2 p = ImVec2(rect_min.x + rect_dim.x/3.0, rect_max.y);
        drawList->AddRectFilled(rect_min, p, color);
    }
    {
        ImVec2 p = ImVec2(rect_max.x - rect_dim.x/3.0, rect_min.y);
        drawList->AddRectFilled(p, rect_max, color);
    }
}

static inline void StopIcon(ImDrawList* drawList, const ImVec2& rect_min, const ImVec2& rect_dim, ImU32 color)
{
    ImVec2 rect_max = ImVec2(rect_min.x + rect_dim.x, rect_min.y + rect_dim.y);
    drawList->AddRectFilled(rect_min, rect_max, color);
}

static inline void ForwardIcon(ImDrawList* drawList, const ImVec2& rect_min, const ImVec2& rect_dim, ImU32 color)
{
    ImVec2 center = ImVec2(rect_min.x + rect_dim.x/2.0, rect_min.y + rect_dim.y/2.0);
    ImVec2 rect_max = ImVec2(rect_min.x + rect_dim.x, rect_min.y + rect_dim.y);
    {
        ImVec2 p3 = ImVec2(rect_min.x, rect_max.y);
        drawList->AddTriangleFilled(rect_min, center, p3, color);
    }
    {
        ImVec2 p1 = ImVec2(center.x, rect_min.y);
        ImVec2 p2 = ImVec2(rect_max.x, center.y);
        ImVec2 p3 = ImVec2(center.x, rect_max.y);
        drawList->AddTriangleFilled(p1, p2, p3, color);
    }
}

static inline void ReverseIcon(ImDrawList* drawList, const ImVec2& rect_min, const ImVec2& rect_dim, ImU32 color)
{
    ImVec2 center = ImVec2(rect_min.x + rect_dim.x/2.0, rect_min.y + rect_dim.y/2.0);
    ImVec2 rect_max = ImVec2(rect_min.x + rect_dim.x, rect_min.y + rect_dim.y);
    {
        ImVec2 p1 = ImVec2(center.x, rect_min.y);
        ImVec2 p2 = ImVec2(rect_min.x, center.y);
        ImVec2 p3 = ImVec2(center.x, rect_max.y);
        drawList->AddTriangleFilled(p1, p2, p3, color);
    }
    {
        ImVec2 p1 = ImVec2(rect_max.x, rect_min.y);
        drawList->AddTriangleFilled(p1, center, rect_max, color);
    }
}

} // namespace ImPop
