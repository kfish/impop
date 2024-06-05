#pragma once

#include "imgui.h"

static inline void RenderScrollableRectangles()
{
    // Number of rectangles
    int num_rects = 10000;
    // Height of each rectangle
    int rect_height = 20;
    // Total height of all rectangles
    int total_height = num_rects * rect_height;

    // Get the initial cursor position before creating the child window
    ImVec2 initial_cursor_pos = ImGui::GetCursorScreenPos();

    // Begin the child window with vertical scrollbar
    ImGui::BeginChild("ScrollableRegion", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // Get the draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Get the position and size of the child window
    ImVec2 child_pos = ImGui::GetCursorScreenPos();
    ImVec2 child_size = ImGui::GetContentRegionAvail();

    // Calculate the nearest scroll position constrained to the box height
    float scroll_y = ImGui::GetScrollY();
    float constrained_scroll_y = rect_height * round(scroll_y / rect_height);
    ImGui::SetScrollY(constrained_scroll_y);

    // Calculate the adjusted child position
    child_pos.y = initial_cursor_pos.y - constrained_scroll_y;

    // Calculate the number of visible rectangles
    int first_visible_rect = static_cast<int>(constrained_scroll_y / rect_height);
    int num_visible_rects = static_cast<int>(child_size.y / rect_height) + 1; // +1 for partial visibility

    // Clamp the range of visible rectangles
    first_visible_rect = std::max(0, first_visible_rect);
    num_visible_rects = std::min(num_visible_rects, num_rects - first_visible_rect);

    // Set the child window size to the total height
    ImGui::Dummy(ImVec2(0.0f, total_height));

    // Ensure consistent starting position
    ImGui::SetCursorScreenPos(child_pos);

    // Render the visible rectangles
    for (int i = 0; i < num_visible_rects; i++)
    {
        int rect_index = first_visible_rect + i;
        ImVec2 rect_min = ImVec2(child_pos.x, child_pos.y + 1 + rect_index * rect_height);
        ImVec2 rect_max = ImVec2(child_pos.x + child_size.x, rect_min.y + rect_height - 2);
        draw_list->AddRect(rect_min, rect_max, IM_COL32(255, 255, 255, 255));

        // Render the text label
        char label[16];
        snprintf(label, sizeof(label), "%d", rect_index + 1);
        draw_list->AddText(ImVec2(rect_min.x + 5, rect_min.y + 2), IM_COL32(255, 255, 0, 255), label);
    }

    ImGui::EndChild();
}

// In your main render loop
static inline void RenderRectangles()
{
    ImGui::Begin("Rectangles");
    RenderScrollableRectangles();
    ImGui::End();
}
