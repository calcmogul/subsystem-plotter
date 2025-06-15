// Copyright (c) Tyler Veness

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include <imgui.h>
#include <implot.h>
#include <wpigui.h>

#include "file_utils.hpp"

int main() {
    // Get list of files in current directory
    std::vector<std::string> files;
    for (const auto& p : std::filesystem::recursive_directory_iterator(".")) {
        if (p.path().string().ends_with(".csv")) {
            files.push_back(p.path().string());
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

    auto timestamps = categorize_files(files);

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
                        ImGui::Checkbox(subsystem.widget_name.c_str(),
                                        &subsystem.is_visible);
                    }
                }
            }
            ImGui::End();
        }

        // Draw visible plots
        for (auto& [timestamp, subsystems] : timestamps) {
            for (auto& subsystem : subsystems) {
                subsystem.plot();
            }
        }
    });
    wpi::gui::Main();

    ImPlot::CreateContext();
    wpi::gui::DestroyContext();

#ifdef _WIN32
    return 0;
#endif
}

#ifdef _WIN32
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* pCmdLine,
                      int nCmdShow) {
    return main();
}
#endif
