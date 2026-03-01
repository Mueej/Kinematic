#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "implot.h"
#include <iostream>

class UI {
private:
    static const int HISTORY_SIZE = 500;
    float speedHistory[HISTORY_SIZE] = { 0 };
    float accXHistory[HISTORY_SIZE] = { 0 };
    float accYHistory[HISTORY_SIZE] = { 0 };
    float chargeHistory[HISTORY_SIZE] = { 0 };
    int historyOffset = 0;

public:
	void Init(GLFWwindow* window);
	void Cleanup();
	void NewFrame();
	void UpdateTelemetry(float speed, float accX, float accY, float charge);
	void Show();
	void Render();
};