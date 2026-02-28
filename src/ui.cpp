#include "ui.hpp"

void UI::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void UI::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::Show(int fps_count, float& size, float* fpsHistory, int historySize, int historyOffset) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.2f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

    ImGui::Begin("My name is window, ImGUI window", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%d", fps_count);
    ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);

    if (ImGui::Button("cmd 1")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 2")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 3")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 4")) {}
    
    if (ImGui::Button("cmd 5")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 6")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 7")) {}
    ImGui::SameLine();
    if (ImGui::Button("cmd 8")) {}

    ImGui::End();

    // Second window for FPS Graph
    ImGui::Begin("FPS Graph", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PlotLines("##fps_graph", fpsHistory, historySize, historyOffset, "FPS", 0.0f, 500.0f, ImVec2(300, 80));
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(9);
}

void UI::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
