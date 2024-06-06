# ImPop

This repository contains useful utilities for
[Dear ImGui](https://github.com/ocornut/imgui) and
[ImPlot](https://github.com/epezent/implot).

`ImPop` should work in the same environments as `ImGui`. Care has been taken to use only
basic C++11 facilities like `constexpr`. It avoids the use of the C++ standard library,
except for the standalone header `impop_ostream.h`.
Please report any build issues or incompatibilities.

## Usage

`ImPop` consists of various (mostly-)independent header files. You can use just the
files you need. Use it as a git submodule or copy individual files into your project.

  - `ImPop::DatePicker`
  - Lightweight config management
  - Color conversion and palette generation
  - `ImPop::OutlineText`
  - `ImPop::PerfFooter`: displays performance metrics

<hr/>

### ImPop::DatePicker

The DatePicker from ImPlot, with a simplified interface that maintains the currently
browsed (not yet selected) date within `ImGui::Storage`.

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

<hr/>

### Lightweight config management

Uses ImGui's ini file for application state. Provides a dropdown menu
to edit config variables.

#### `app_config.h`

Your custom config must be a POD struct containing only a sequence of
`ImPop::ConfigItem`.

The given string will be used as a config key in the ini file, and also as the
text for the menu.

```cpp
#include "impop_config.h"

struct Config {
    ImPop::ConfigItem use_foo_bar{"Use Foo and Bar", true};
    ImPop::ConfigItem baz_coeff{"BazCoefficient (tm)", 3.14f};
    ImPop::ConfigItem count_quux{"Number of Quux", 7};
} config;

```

#### `main.cpp`

Initialize the config manager, and read any stored config values. Note that you
must call `ImPop::InitializeConfig(config)` during a Frame, as it needs
to access storage via the current window in the ImGui::Context. As you usually
only want to load the ini file once on application startup, setup a frame before
entering your main application loop.

```cpp
#include "impop_config.h"
#include "myconfig.h"

// ...

    //ImGui_Impl*_NewFrame(); // Set the frame dimensions
    ImGui::NewFrame();        // Create a window for GetID()
    ImPop::InitializeConfig(config);
    ImGui::Render();

```

#### `app.cpp`

In your application code, read and write your config values directly from your `struct Config`:

```cpp
#include "app_config.h"

if (config.use_foo_bar.bool_value) {
    // Do stuff with foo and bar
}

int cakes = config.count_quux.int_value * 3; // 3 cakes each

// Update the current estimate
config.baz_coeff.float_value = 3.1417;

```

#### `app_gui.cpp`

Display a menu for setting the config values:

```cpp
#include "impop_config.h"

if (ImGui::BeginMenu("My Application")) {
    ImPop::ConfigMenu();
    ImGui::EndMenu();
}

```

<hr/>

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

<hr/>

### ImPop::OutlineText

White text with a black outline.


```cpp
#include "impop_text.h"

//...

ImPop::OutlineText(draw_list, ImVec2(pos.x, pos.y), "Welcome %s", name);

```

<hr/>

### ImPop::PerfFooter

This provides a handy footer displaying performance metrics:

  * Memory Usage: Arena, Free Space, Allocated Space
  * Memory Fragmentation
  * Current Framerate (FPS)

```cpp
#include "impop_footer.h"

//...

ImPop::PerfFooter();
```

<hr/>

License
-------

ImPop is licensed under the MIT License, see [LICENSE.txt](https://github.com/kfish/impop/blob/master/LICENSE.txt) for more information.
