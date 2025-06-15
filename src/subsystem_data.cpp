// Copyright (c) Tyler Veness

#include "subsystem_data.hpp"

#include <format>

#include <imgui.h>
#include <implot.h>

SubsystemData::SubsystemData(std::string_view subsystem,
                             std::string_view timestamp)
    : subsystem{subsystem},
      widget_name{std::format("{}##{}", subsystem, timestamp)},
      window_title{std::format("{} ({})", subsystem, timestamp)} {}

void SubsystemData::plot() {
    if (!is_visible) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(640, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(window_title.c_str())) {
        // TODO: Loop over units, making a new plot for each. See open_figs() in
        // subsystem_plotter.py.
        if (ImPlot::BeginPlot("Title")) {
            // float x_data[1000] = ...;
            // float y_data[1000] = ...;
            // ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
}
