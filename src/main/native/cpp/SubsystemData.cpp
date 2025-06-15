// Copyright (c) 2020-2025 FRC Team 3512. All Rights Reserved.

#include "SubsystemData.hpp"

#include <format>

#include <imgui.h>
#include <implot.h>

SubsystemData::SubsystemData(std::string_view subsystem,
                             std::string_view timestamp)
    : subsystem{subsystem},
      widgetName{std::format("{}##{}", subsystem, timestamp)},
      windowTitle{std::format("{} ({})", subsystem, timestamp)} {}

void SubsystemData::Plot() {
    if (!isVisible) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(640, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(windowTitle.c_str())) {
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
