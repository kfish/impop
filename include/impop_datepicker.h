#pragma once

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

namespace ImPop {

constexpr int SECONDS_IN_A_DAY = 86400;

static constexpr int time_t_to_days(time_t t) {
    return t / SECONDS_IN_A_DAY;
}

static constexpr time_t days_to_time_t(int days) {
    return days * SECONDS_IN_A_DAY;
}

static constexpr int implot_time_to_days(const ImPlotTime& t) {
    return time_t_to_days(t.S);
}

static inline ImPlotTime days_to_implot_time(int days) {
    return ImPlotTime(days_to_time_t(days), 0);
}

static inline ImPlotTime LocTimeNow() {
    time_t now = time(NULL);
    struct tm now_tm;
    localtime_r(&now, &now_tm);
    return ImPlot::MkLocTime(&now_tm);
}

static inline ImPlotTime GmtTimeNow() {
    time_t now = time(NULL);
    struct tm now_tm;
    gmtime_r(&now, &now_tm);
    return ImPlot::MkGmtTime(&now_tm);
}

static inline bool DatePicker(const char* id, ImPlotTime* t, const ImPlotTime* default_time = nullptr, const ImPlotTime* min_time = nullptr, const ImPlotTime* max_time = nullptr)
{
    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiStorage* storage = ImGui::GetStateStorage();

    ImGuiID key = ImGui::GetID(id);
    int currentDays = storage->GetInt(key, implot_time_to_days(t->S ? *t : *default_time));
    ImPlotTime currentT = days_to_implot_time(currentDays);

    int level = 0;
    bool pressed = ImPlot::ShowDatePicker("##date", &level, &currentT, min_time, max_time);
    if (pressed) {
        *t = currentT;
    } else {
        currentDays = implot_time_to_days(currentT);
        storage->SetInt(key, currentDays);
    }

    return pressed;
}

} // namespace ImPop
