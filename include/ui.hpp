#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>

class UI {
public:
	void Init(GLFWwindow* window);
	void Cleanup();
	void NewFrame();
	void Show(int fps_count, float& size, float* fpsHistory, int historySize, int historyOffset);
	void Render();
};