#include "ui.hpp"
#include "implot.h"

void UI::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    ImPlot::CreateContext();
}

void UI::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::UpdateTelemetry(float speed, float accX, float accY, float charge) {
    speedHistory[historyOffset] = speed;
    accXHistory[historyOffset] = accX;
    accYHistory[historyOffset] = accY;
    chargeHistory[historyOffset] = charge;
    historyOffset = (historyOffset + 1) % HISTORY_SIZE;
}

void UI::Show() {
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.2f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = io.DisplaySize.x;
    ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(windowWidth, 0.0f), ImVec2(windowWidth, FLT_MAX));
    ImGui::Begin("Commands", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

    const char* buttons[] = {"cmd 1", "cmd 2", "cmd 3", "cmd 4", "cmd 5", "cmd 6", "cmd 7", "cmd 8"};
    int numButtons = 8;
    float totalButtonWidth = 0.0f;
    for (int i = 0; i < numButtons; ++i) {
        totalButtonWidth += ImGui::CalcTextSize(buttons[i]).x + ImGui::GetStyle().FramePadding.x * 0.0f;
        if (i < numButtons - 1) totalButtonWidth += ImGui::GetStyle().ItemSpacing.x;
    }

    ImGui::SetCursorPosX((windowWidth - totalButtonWidth) * 0.5f);
    for (int i = 0; i < numButtons; ++i) {
        if (ImGui::Button(buttons[i])) {}
        if (i < numButtons - 1) ImGui::SameLine();
    }

    ImGui::Spacing();
    
    float plotWidth = windowWidth * 0.95f;
    ImGui::SetCursorPosX((windowWidth - plotWidth) * 0.5f);
    
    if (ImPlot::BeginSubplots("##Graphs", 1, 4, ImVec2(plotWidth, 200))) {
        
        if (ImPlot::BeginPlot("Speed")) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
            ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 120.0, ImPlotCond_Always);
            ImPlot::PlotLine("Speed", speedHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Acceleration")) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
            ImPlot::SetupAxesLimits(0, HISTORY_SIZE, -50.0, 50.0, ImPlotCond_Always);
            ImPlot::PlotLine("Acc X", accXHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
            ImPlot::PlotLine("Acc Y", accYHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Charge")) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
            ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 100.0, ImPlotCond_Always);
            ImPlot::PlotLine("Charge", chargeHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Charge")) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
            ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 100.0, ImPlotCond_Always);
            ImPlot::PlotLine("Charge", chargeHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
            ImPlot::EndPlot();
        }

        ImPlot::EndSubplots();
    }

    ImGui::End();
    // ImGui::PopStyleVar();
    ImGui::PopStyleColor(9);
}

void UI::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Cleanup() {
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
