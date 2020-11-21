// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <imgui.h>
#include <wpigui.h>

#include <cmath>

namespace gui = wpi::gui;

int main() {
    gui::CreateContext();
    gui::Initialize("Subsystem plotter", 1024, 768);
    gui::Texture tex;
    gui::AddEarlyExecute([&] {
        ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Log Viewer")) {
            if (ImGui::CollapsingHeader("<Timestamp 1 here>")) {
                ImGuiIO& io = ImGui::GetIO();

                if (ImGui::TreeNode("<Subsystem 1 here>##1")) {
                    ImGui::CheckboxFlags("<Unit 1 here>##1",
                                         (unsigned int*)&io.ConfigFlags,
                                         ImGuiConfigFlags_NavEnableKeyboard);
                    ImGui::CheckboxFlags("<Unit 2 here>##1",
                                         (unsigned int*)&io.ConfigFlags,
                                         ImGuiConfigFlags_NavEnableGamepad);
                    ImGui::TreePop();
                    ImGui::Separator();
                }

                if (ImGui::TreeNode("<Subsystem 2 here>##2")) {
                    ImGuiBackendFlags backend_flags =
                        io.BackendFlags;  // Make a local copy to avoid
                                          // modifying actual back-end flags.
                    ImGui::CheckboxFlags("io.BackendFlags: HasGamepad",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasGamepad);
                    ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasMouseCursors);
                    ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasSetMousePos);
                    ImGui::CheckboxFlags(
                        "io.BackendFlags: RendererHasVtxOffset",
                        (unsigned int*)&backend_flags,
                        ImGuiBackendFlags_RendererHasVtxOffset);
                    ImGui::TreePop();
                }
            }

            if (ImGui::CollapsingHeader("<Timestamp 2 here>")) {
                ImGuiIO& io = ImGui::GetIO();

                if (ImGui::TreeNode("<Subsystem 1 here>")) {
                    ImGui::CheckboxFlags("<Unit 1 here>",
                                         (unsigned int*)&io.ConfigFlags,
                                         ImGuiConfigFlags_NavEnableKeyboard);
                    ImGui::CheckboxFlags("<Unit 2 here>",
                                         (unsigned int*)&io.ConfigFlags,
                                         ImGuiConfigFlags_NavEnableGamepad);
                    ImGui::TreePop();
                    ImGui::Separator();
                }

                if (ImGui::TreeNode("<Subsystem 2 here>")) {
                    ImGuiBackendFlags backend_flags =
                        io.BackendFlags;  // Make a local copy to avoid
                                          // modifying actual back-end flags.
                    ImGui::CheckboxFlags("io.BackendFlags: HasGamepad",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasGamepad);
                    ImGui::CheckboxFlags("io.BackendFlags: HasMouseCursors",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasMouseCursors);
                    ImGui::CheckboxFlags("io.BackendFlags: HasSetMousePos",
                                         (unsigned int*)&backend_flags,
                                         ImGuiBackendFlags_HasSetMousePos);
                    ImGui::CheckboxFlags(
                        "io.BackendFlags: RendererHasVtxOffset",
                        (unsigned int*)&backend_flags,
                        ImGuiBackendFlags_RendererHasVtxOffset);
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    });
    gui::Main();
}
