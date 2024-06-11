#pragma once

#include "imgui.h"

namespace ImPop {

enum class OSDType {
    None=0,
    Text,
    Play,
    Pause,
    Stop,
    Forward,
    Reverse
};

inline OSDType osd_type = OSDType::None;
inline char osd_text_buffer[1024];
inline ImVec2 osd_text_pos;
inline float osd_font_size;
inline int osd_forward_reverse_speed = 1;

inline float osd_start = 0;
inline float osd_timeout = 0.3;

static inline void OSDClear() {
    osd_start = 0;
    osd_type = OSDType::None;
}

static inline void OSDTextV(const char* fmt, va_list args) {
    osd_type = OSDType::Text;
    osd_start = ImGui::GetTime();

    vsnprintf(osd_text_buffer, sizeof(osd_text_buffer), fmt, args);

    ImGuiIO& io = ImGui::GetIO();
    float max_text_width = io.DisplaySize.x * 0.9f; // Let's use 90% of the screen width for text
    ImFont* font = ImGui::GetFont(); // Get the default font

    // Start with a large font size
    osd_font_size = 800.0f;
    ImVec2 text_size = font->CalcTextSizeA(osd_font_size, FLT_MAX, 0.0f, osd_text_buffer);

    // Adjust font size to fit the text within the screen width
    while (text_size.x > max_text_width && osd_font_size > 1.0f) {
        osd_font_size -= 1.0f;
        text_size = font->CalcTextSizeA(osd_font_size, FLT_MAX, 0.0f, osd_text_buffer);
    }

    // Calculate the position to center the text
    osd_text_pos = ImVec2((io.DisplaySize.x - text_size.x) * 0.5f, (io.DisplaySize.y - text_size.y) * 0.5f);
}

static inline void OSDText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    OSDTextV(fmt, args);
    va_end(args);
}

static inline void OSDPlay() {
    osd_type = OSDType::Play;
    osd_start = ImGui::GetTime();
}

static inline void OSDPause() {
    osd_type = OSDType::Pause;
    osd_start = ImGui::GetTime();
}

static inline void OSDStop() {
    osd_type = OSDType::Stop;
    osd_start = ImGui::GetTime();
}

static inline void OSDForward() {
    osd_type = OSDType::Forward;
    osd_start = ImGui::GetTime();
}

static inline void OSDReverse() {
    osd_type = OSDType::Reverse;
    osd_start = ImGui::GetTime();
}

static inline void OSDShow() {
    if (osd_type == OSDType::None) return;

    // Update timeout
    float timer = ImGui::GetTime() - osd_start;
    if (timer > osd_timeout) {
        OSDClear();
        return;
    }

    float t = 1.0 - (timer / osd_timeout);

    ImU32 alpha = 128 * t;
    ImU32 color = IM_COL32(255, 255, 255, alpha);
    ImU32 outline_color = IM_COL32(0, 0, 0, alpha);

    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 sz = io.DisplaySize;

    const ImVec2 center = ImVec2(sz.x/2.0, sz.y/2.0);
    const float side = std::min(center.x, center.y);
    const ImVec2 square = ImVec2(side, side);
    const ImVec2 square_min = ImVec2(center.x - side/2.0, center.y - side/2.0);
    const ImVec2 square_max = ImVec2(center.x + side/2.0, center.y + side/2.0);

    // Access the foreground draw list (drawn after all windows)
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    switch (osd_type) {
        case OSDType::None:
            break;
        case OSDType::Text:
            draw_list->AddText(nullptr, osd_font_size, osd_text_pos, color, osd_text_buffer);
            break;
        case OSDType::Play:
            {
                ImVec2 p2 = ImVec2(square_max.x, center.y);
                ImVec2 p3 = ImVec2(square_min.x, square_max.y);
                draw_list->AddTriangleFilled(square_min, p2, p3, color);
            }
            break;
        case OSDType::Pause:
            {
                ImVec2 p = ImVec2(square_min.x + side/3.0, square_max.y);
                draw_list->AddRectFilled(square_min, p, color);
            }
            {
                ImVec2 p = ImVec2(square_max.x - side/3.0, square_min.y);
                draw_list->AddRectFilled(p, square_max, color);
            }
            break;
        case OSDType::Stop:
            {
                draw_list->AddRectFilled(square_min, square_max, color);
            }
            break;
        case OSDType::Forward:
            {
                ImVec2 p3 = ImVec2(square_min.x, square_max.y);
                draw_list->AddTriangleFilled(square_min, center, p3, color);
            }
            {
                ImVec2 p1 = ImVec2(center.x, square_min.y);
                ImVec2 p2 = ImVec2(square_max.x, center.y);
                ImVec2 p3 = ImVec2(center.x, square_max.y);
                draw_list->AddTriangleFilled(p1, p2, p3, color);
            }
            break;
        case OSDType::Reverse:
            {
                ImVec2 p1 = ImVec2(center.x, square_min.y);
                ImVec2 p2 = ImVec2(square_min.x, center.y);
                ImVec2 p3 = ImVec2(center.x, square_max.y);
                draw_list->AddTriangleFilled(p1, p2, p3, color);
            }
            {
                ImVec2 p1 = ImVec2(square_max.x, square_min.y);
                draw_list->AddTriangleFilled(p1, center, square_max, color);
            }
            break;
        default:
            break;
    }
}

} // namespace ImPop
