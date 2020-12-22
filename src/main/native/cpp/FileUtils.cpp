// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include "FileUtils.hpp"

#include <filesystem>
#include <fstream>
#include <regex>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

namespace fs = std::filesystem;

void SubsystemData::Plot() {
    if (!isVisible) {
        return;
    }

    fmt::print("timestamp={}\n", timestamp);
    fmt::print("subsystem={}\n", subsystem);

    ImGui::SetNextWindowPos(ImVec2(640, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    std::string title =
        fmt::format("{} ({})", subsystem.data(), timestamp.data());
    if (ImGui::Begin(title.c_str())) {
        if (ImPlot::BeginPlot("Title")) {
            fmt::print("draw\n");
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
}

int NumLines(std::string_view filename) {
    std::ifstream file{filename.data()};
    std::string line;

    int lines = 0;
    while (std::getline(file, line)) {
        ++lines;
    }

    return lines;
}

std::map<std::string, std::map<std::string, SubsystemData>> CategorizeFiles(
    const std::vector<std::string>& files) {
    // First group is name, second group is data type, and third group is date
    std::regex fileRgx{
        "^\\..*?/([A-Za-z ]+) (states|inputs|outputs)-"
        "(\\d{4}-\\d{2}-\\d{2}-\\d{2}_\\d{2}_\\d{2})\\.csv$"};

    std::map<std::string, std::map<std::string, SubsystemData>> timestamps;

    // Sorting the file list puts files into the order["inputs", "outputs",
    // "states"]. This means data series will be loaded in the order of
    // ["inputs", "outputs", "references", "states"] (references are logged
    // before states). This produces the desired dataset layering on plots.
    for (const auto& file : files) {
        std::smatch match;
        if (!std::regex_search(file, match, fileRgx)) {
            continue;
        }

        // If file is empty or only has header (that is, has no data), ignore
        // it. We ignore the case of one line of data because it might be
        // truncated.
        if (NumLines(file) <= 2) {
            continue;
        }

        // If the file is a CSV with the correct name pattern, add it to the
        // list for the associated timestamp.
        auto timestamp = match[3];
        auto name = fs::path{match[1]}.filename();
        timestamps[timestamp][name].filenames.push_back(file);
    }

    return timestamps;
}
