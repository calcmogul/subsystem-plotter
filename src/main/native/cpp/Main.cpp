// Copyright (c) 2020-2025 FRC Team 3512. All Rights Reserved.

#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <imgui.h>
#include <implot.h>
#include <wpigui.h>

#include "FileUtils.hpp"

namespace fs = std::filesystem;

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

    // Sorting the file list puts files into the following order:
    //
    // ["inputs", "outputs", "states"]
    //
    // This means data series will be loaded in the following order, assuming
    // references are logged before states:
    //
    // ["inputs", "outputs", "references", "states"]
    //
    // This produces the desired dataset layering on plots.
    std::sort(files.begin(), files.end());

    auto timestamps = CategorizeFiles(files);

    wpi::gui::CreateContext();
    ImPlot::CreateContext();

    // Create a unique INI
    wpi::gui::AddInit(
        [] { ImGui::GetIO().IniFilename = "subsystem-plotter.ini"; });

    wpi::gui::Initialize("Subsystem plotter", 1280, 720);

    wpi::gui::AddEarlyExecute([&] {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Log Viewer")) {
            for (auto& [timestamp, subsystems] : timestamps) {
                if (ImGui::CollapsingHeader(timestamp.c_str(),
                                            ImGuiTreeNodeFlags_DefaultOpen)) {
                    for (auto& subsystem : subsystems) {
                        ImGui::Checkbox(subsystem.widgetName.c_str(),
                                        &subsystem.isVisible);
                    }
                }
            }
            ImGui::End();
        }

        // Draw visible plots
        for (auto& [timestamp, subsystems] : timestamps) {
            for (auto& subsystem : subsystems) {
                subsystem.Plot();
            }
        }
    });
    wpi::gui::Main();

    ImPlot::CreateContext();
    wpi::gui::DestroyContext();
}
