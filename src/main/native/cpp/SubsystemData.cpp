// Copyright (c) 2020-2021 FRC Team 3512. All Rights Reserved.

#include <fstream>
#include <regex>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "FileUtils.hpp"

/**
 * Splits string into array of substrings using the given delimiter.
 *
 * @param str   String to split.
 * @param delim Substring delimiter.
 */
std::vector<std::string> Split(const std::string& str, std::string_view delim) {
    std::regex splitRgx{delim.data()};
    return std::vector<std::string>{
        std::sregex_token_iterator{str.begin(), str.end(), splitRgx, -1},
        std::sregex_token_iterator{}};
}

struct DataSeries {
    std::string xlabel;
    std::vector<double> xdata;
    std::string ylabel;
    std::vector<double> ydata;
};

SubsystemData::SubsystemData(std::string_view subsystem,
                             std::string_view timestamp)
    : subsystem{subsystem},
      widgetName{fmt::format("{}##{}", subsystem, timestamp)},
      windowTitle{fmt::format("{} ({})", subsystem, timestamp)} {
    // First group is name and second group is unit
    std::regex unitRgx{"^([\\w\\- ]+) \\((.*?)\\)$"};

    // Extract data columns from files and make groups of them keyed on their
    // unit
    std::string line;
    for (const auto& filename : filenames) {
        std::ifstream file{filename};

        // Get labels from first row of file
        std::getline(file, line);
        std::vector<std::string> labels{Split(line, ",")};

        // Strip quotes from around labels
        for (auto& label : labels) {
            if (label.front() == '"') {
                label.replace(0, 1, "");
            }
            if (label.back() == '"') {
                label.pop_back();
            }
        }

        // Retrieve data from remaining rows of file. "skip_footer=1" skips the
        // last line because it may be incompletely written.
        std::vector<std::vector<double>> data;
        while (std::getline(file, line)) {
            data.emplace_back();
            for (const auto& elem : Split(line, ",")) {
                // FIXME: breaks if elem is an empty string
                data.back().emplace_back(std::stod(elem));
            }
        }
    }
}

void SubsystemData::Plot() {
    if (!isVisible) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(640, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(windowTitle.c_str())) {
        // TODO: Loop over units, making a new plot for each. See open_figs() in
        // subsystem_plotter.py.
        // if (ImPlot::BeginPlot(
        //         fmt::format("{} ({})", subsystem, unit).c_str())) {
        if (ImPlot::BeginPlot("Title")) {
            // float x_data[1000] = ...;
            // float y_data[1000] = ...;
            // ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
}
