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
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(172.0f/255.0f, 211.0f/255.0f, 230.0f/255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(190.0f/255.0f, 225.0f/255.0f, 240.0f/255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = io.DisplaySize.x;
    float gap = 20.0f;
    
    int numPlots = 4;
    float plotWindowWidth = (windowWidth - gap * (numPlots + 1)) / numPlots;

    const char* buttons[] = {"cmd 1", "cmd 2", "cmd 3", "cmd 4", "cmd 5", "cmd 6", "cmd 7", "cmd 8"};
    int numButtons = 8;
    
    ImVec2 buttonSize(100.0f, 30.0f); // Make buttons larger
    ImGuiWindowFlags buttonWindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
    float buttonGap = 20.0f; // Increased gap specific to buttons
    float currentButtonX = windowWidth * 0.5f - ((buttonSize.x * numButtons + buttonGap * (numButtons - 1)) * 0.5f);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    
    for (int i = 0; i < numButtons; ++i) {
        ImGui::SetNextWindowPos(ImVec2(currentButtonX, gap), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
        
        char windowName[32];
        snprintf(windowName, sizeof(windowName), "CommandWindow%d", i);
        
        ImGui::Begin(windowName, NULL, buttonWindowFlags);
        
        ImVec2 btnMin = ImGui::GetCursorScreenPos();
        ImVec2 btnMax = ImVec2(btnMin.x + buttonSize.x, btnMin.y + buttonSize.y);
        bool hovered = ImGui::IsMouseHoveringRect(btnMin, btnMax);
        
        if (hovered) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        
        if (ImGui::Button(buttons[i], buttonSize)) {}
        
        if (hovered) {
            ImGui::PopStyleColor();
        }
        
        ImGui::End();
        
        currentButtonX += buttonSize.x + buttonGap;
    }
    
    ImGui::PopStyleVar(2);

    ImGuiWindowFlags plotFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings;
    float currentX = gap;
    
    float windowHeight = 200.0f;
    float pad = 10.0f;

    ImGui::SetNextWindowPos(ImVec2(currentX, io.DisplaySize.y - gap), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(plotWindowWidth, windowHeight));
    ImGui::Begin("SpeedPlot", NULL, plotFlags);
    if (ImPlot::BeginPlot("Speed", ImVec2(plotWindowWidth - pad * 2, windowHeight - pad * 2))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 120.0, ImPlotCond_Always);
        ImPlot::PlotLine("Speed", speedHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
        ImPlot::EndPlot();
    }
    ImGui::End();
    currentX += plotWindowWidth + gap;

    ImGui::SetNextWindowPos(ImVec2(currentX, io.DisplaySize.y - gap), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(plotWindowWidth, windowHeight));
    ImGui::Begin("AccelPlot", NULL, plotFlags);
    if (ImPlot::BeginPlot("Acceleration", ImVec2(plotWindowWidth - pad * 2, windowHeight - pad * 2))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxesLimits(0, HISTORY_SIZE, -50.0, 50.0, ImPlotCond_Always);
        ImPlot::PlotLine("Acc X", accXHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
        ImPlot::PlotLine("Acc Y", accYHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
        ImPlot::EndPlot();
    }
    ImGui::End();
    currentX += plotWindowWidth + gap;

    ImGui::SetNextWindowPos(ImVec2(currentX, io.DisplaySize.y - gap), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(plotWindowWidth, windowHeight));
    ImGui::Begin("Charge1Plot", NULL, plotFlags);
    if (ImPlot::BeginPlot("Charge 1", ImVec2(plotWindowWidth - pad * 2, windowHeight - pad * 2))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 100.0, ImPlotCond_Always);
        ImPlot::PlotLine("Charge", chargeHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
        ImPlot::EndPlot();
    }
    ImGui::End();
    currentX += plotWindowWidth + gap;

    ImGui::SetNextWindowPos(ImVec2(currentX, io.DisplaySize.y - gap), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(plotWindowWidth, windowHeight));
    ImGui::Begin("Charge2Plot", NULL, plotFlags);
    if (ImPlot::BeginPlot("Charge 2", ImVec2(plotWindowWidth - pad * 2, windowHeight - pad * 2))) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxesLimits(0, HISTORY_SIZE, 0.0, 100.0, ImPlotCond_Always);
        ImPlot::PlotLine("Charge", chargeHistory, HISTORY_SIZE, 1.0, 0.0, { ImPlotProp_Offset, historyOffset });
        ImPlot::EndPlot();
    }
    ImGui::End();
    ImGui::PopStyleVar();
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
