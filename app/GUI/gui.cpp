#include "gui.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void GUI::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::someWindow(
    const char* title,
    float x_pos, float y_pos,
    float width, float length
) {
    if (x_pos != -1 && y_pos != -1) {
        ImGui::SetNextWindowPos(ImVec2(x_pos, y_pos));
    }
    if (width > 0 && length > 0) {
        ImGui::SetNextWindowSize(ImVec2(width, length));
    }
    
    ImGui::Begin(title, NULL);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}