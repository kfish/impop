# ImPop

This repository contains useful utilities for
[Dear ImGui](https://github.com/ocornut/imgui) and
[ImPlot](https://github.com/epezent/implot).

## Usage

`ImPop` consists of various (mostly-)independent header files. Use it as a git submodule
or copy whichever files you need into your project.

  - `ImPop::DatePicker`
  - Color conversion and palette generation
  - `ImPop::OutlineText`
  - `ImPop::PerfFooter`: displays performance metrics

### ImPop::DatePicker

```cpp
#include "impop_datepicker.h"

// ...

ImPlotTime t; // current value
ImPlotTime default_time = ImPlot::MakeTime(2024, 1, 1);
ImPlotTime min_time = ImPlot::MakeTime(2023, 1, 1);
ImPlotTime max_time = ImPop::LocTimeNow();

if (ImPop::DatePicker("date", &t, &default_time, &min_time, &max_time)) {
    // `t` has been updated
}
```

### Color conversion and palette generation

These are declared constexpr, so they can be used in constructor initializers, or
defined statically and evaluated at compile-time.


```cpp
#include "impop_color.h"

// ...


// Color 
static constexpr ImU32 red = IM_COL32(246, 70, 93, 255);
static constexpr ImU32 orange = IM_COL32(163, 129, 17, 255);
static constexpr ImU32 green = IM_COL32(46, 189, 133, 255);

// Compile-time color conversions
static constexpr ImVec4 red_vec4 = ImPop::ColorConvertU32ToFloat4(red);
static constexpr ImVec4 orange_vec4 = ImPop::ColorConvertU32ToFloat4(orange);
static constexpr ImVec4 green_vec4 = ImPop::ColorConvertU32ToFloat4(green);

// Compile-time palette generation
static constexpr size_t palette_size = 16;
static constexpr ImVec4 color_0 = red_vec4;
static constexpr ImVec4 color_1 = ImPop::AdjustBrightness(orange_vec4, 0.6);
static constexpr ImVec4 color_2 = ImPop::AdjustBrightness(ImPop::AdjustSaturation(green_vec4, 0.8), 0.8);
static constexpr std::array<ImU32, palette_size> traffic_lights =
        ImPop::GeneratePalette<palette_size>(color_0, color_1, color_2);
```

### ImPop::OutlineText

White text with a black outline.


```cpp
#include "impop_text.h"

//...

ImPop::OutlineText(draw_list, ImVec2(pos.x, pos.y), "Welcome %s", name);

```

### ImPop::PerfFooter

This provides a handy footer displaying performance metrics:
    - Memory Usage: Arena, Free Space, Allocated Space
    - Memory Fragmentation
    - Current Framerate (FPS)

```cpp
#include "impop_footer.h"

//...

ImPop::PerfFooter();
```

### Dependencies

`ImPop` should work in the same environments as `ImGui`. Please report any build issues or
incompatibilities.

`ImPop::DatePicker` requires ImPlot; it is a wrapper around `ImPlot::ShowDatePicker`,
maintaining the currently browsed (not yet selected) date within `ImGui::Storage`.

License
-------

ImPop is licensed under the MIT License, see [LICENSE.txt](https://github.com/kfish/impop/blob/master/LICENSE.txt) for more information.
