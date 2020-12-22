// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <wpigui.h>

#include "FileUtils.hpp"

namespace fs = std::filesystem;

/**
 * Draw plots for a given subsystem.
 *
 * @param subsystem Subsystem name.
 * @param plotData  Plot data including a list of CSV data files for the
 *                  subsystem.
 */
void DrawPlots(std::string_view subsystem, const PlotData& plotData) {
    if (ImPlot::BeginPlot("Title")) {
        fmt::print("draw\n");
    }
}

#ifdef _WIN32
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* pCmdLine,
                      int nCmdShow) {
#else
int main() {
#endif
    // Get list of files in current directory
    std::vector<std::string> files;
    for (const auto& p : fs::recursive_directory_iterator(".")) {
        if (std::string_view{p.path().string()}.ends_with(".csv")) {
            files.push_back(p.path());
        }
    }
    std::sort(files.begin(), files.end());

    auto timestamps = CategorizeFiles(files);

    wpi::gui::CreateContext();
    ImPlot::CreateContext();

    // Create a unique INI
    wpi::gui::AddInit(
        [] { ImGui::GetIO().IniFilename = "subsystem-plotter.ini"; });

    wpi::gui::Initialize("Subsystem plotter", 1024, 768);

    wpi::gui::AddEarlyExecute([&] {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Log Viewer")) {
            for (auto& [timestamp, subsystems] : timestamps) {
                if (ImGui::CollapsingHeader(timestamp.c_str(),
                                            ImGuiTreeNodeFlags_DefaultOpen)) {
                    for (auto& [subsystem, plotData] : subsystems) {
                        plotData.widgetName =
                            fmt::format("{}##{}", subsystem, timestamp);
                        ImGui::Checkbox(plotData.widgetName.c_str(),
                                        &plotData.isVisible);
                    }
                }
            }
        }
        ImGui::End();

        // Draw visible plots
        for (auto& [timestamp, subsystems] : timestamps) {
            for (auto& [subsystem, plotData] : subsystems) {
                // Show plot if it's enabled
                if (plotData.isVisible) {
                    fmt::print("timestamp={}\n", timestamp);
                    fmt::print("subsystem={}\n", subsystem);

                    ImGui::SetNextWindowPos(ImVec2(640, 0),
                                            ImGuiCond_FirstUseEver);
                    ImGui::SetNextWindowSize(ImVec2(640, 480),
                                             ImGuiCond_FirstUseEver);
                    ImGui::Begin(fmt::format("{} ({})", subsystem.data(),
                                             timestamp.data())
                                     .data());
                    DrawPlots(subsystem,
                              timestamps[timestamp.data()][subsystem.data()]);
                    ImGui::End();
                }
            }
        }
    });
    wpi::gui::Main();

    ImPlot::CreateContext();
    wpi::gui::DestroyContext();
}
