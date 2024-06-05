#pragma once

#include <malloc.h>

#include "imgui.h"
#include "impop_text.h"

namespace ImPop {

template<typename T>
static inline std::string readable_bytes(T num) {
    char buffer[64];

    if (num >= 1e15) {
        snprintf(buffer, sizeof(buffer), "%.2f PB", (float)num / 1e15);
    } else if (num >= 1e12) {
        snprintf(buffer, sizeof(buffer), "%.2f TB", (float)num / 1e12);
    } else if (num >= 1e9) {
        snprintf(buffer, sizeof(buffer), "%.2f GB", (float)num / 1e9);
    } else if (num >= 1e6) {
        snprintf(buffer, sizeof(buffer), "%.2f MB", (float)num / 1e6);
    } else if (num >= 1e3) {
        snprintf(buffer, sizeof(buffer), "%.2f kB", (float)num / 1e3);
    } else {
        snprintf(buffer, sizeof(buffer), "%.2f B", (float)num);
    }

    return std::string(buffer);
}

static inline void PerfFooter() {
    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 screenSize = io.DisplaySize;

    // Access the foreground draw list (drawn after all windows)
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Set the rectangle and text parameters
    const ImVec2 footerSize(screenSize.x, 20);  // Adjust the height as needed
    const ImVec2 footerPos(0, screenSize.y - footerSize.y);
    const ImU32 backgroundColor = IM_COL32(0, 0, 0, 102);  // Semi-transparent black

    // Draw the rectangle with the specified color
    draw_list->AddRectFilled(footerPos, footerPos + footerSize, backgroundColor);

    struct mallinfo2 mi = mallinfo2();
    size_t totalFreeSpace = mi.fordblks;
    size_t totalAllocatedSpace = mi.uordblks;
    double fragmentationRatio = static_cast<double>(totalFreeSpace) / (totalAllocatedSpace + totalFreeSpace);

    OutlineText(draw_list, ImVec2(screenSize.x-700, footerPos.y + 3),
                "%9s arena | %9s free | %9s alloc | %4.1f%% frag",
                readable_bytes(mi.arena).c_str(),
                readable_bytes(totalFreeSpace).c_str(),
                readable_bytes(totalAllocatedSpace).c_str(),
                fragmentationRatio*100.0);

    OutlineText(draw_list, ImVec2(screenSize.x-60, footerPos.y + 3),
                "%.1f FPS", io.Framerate);
}

} // namespace ImPop
