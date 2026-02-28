#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include "elements.hpp"
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "camera.hpp"
#include "EBO.hpp"
#include "ui.hpp"

int main() {
    Elements elements(100, 1.5f, 0.5f, 2.0f);

    unsigned int width = 800;
    unsigned int height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Kinematics", NULL, NULL);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwMakeContextCurrent(window);

    gladLoadGL();
	glViewport(0, 0, width, height);
    Shader shaderProgram("../resoureces/shaders/default.vert", "../resoureces/shaders/default.frag");

    VAO vao;
    vao.Bind();
    VBO vbo(elements.axes, sizeof(elements.axes));
    vao.LinkVBO(vbo, 0, 3);
    vao.Unbind();
    vbo.Unbind();

    VAO vao2;
    vao2.Bind();
    VBO vbo2(elements.cone, sizeof(elements.cone));
    EBO ebo(elements.Indices, sizeof(elements.Indices));
    vao2.LinkVBO(vbo2, 0, 3);
    vao2.Unbind();
    vbo2.Unbind();
    ebo.Unbind();

    VAO vao3;
    vao3.Bind();
    VBO vbo3(elements.car, sizeof(elements.car));
    EBO ebo2(elements.Indices, sizeof(elements.Indices));
    vao3.LinkVBO(vbo3, 0, 3);
    vao3.Unbind();
    vbo3.Unbind();
    ebo2.Unbind();

    Camera camera(width, height, 10.0f);
	glfwSetWindowUserPointer(window, &camera);

	glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        if (ImGui::GetIO().WantCaptureMouse) return;
		Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
		bool shift = glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		cam->ScrollZoom(yoffset, shift);
	});

    glEnable(GL_DEPTH_TEST);

    UI ui;
    ui.Init(window);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float size = 1.0f;

    int colorLoc = glGetUniformLocation(shaderProgram.ID, "color");
    int offsetLoc = glGetUniformLocation(shaderProgram.ID, "offset");
    int camMatrixLoc = glGetUniformLocation(shaderProgram.ID, "camMatrix");

    float fpstime = 0.0f;
    int cnt = 0;
    int fps_count = 0;

    // FPS History for graph
    const int FPS_HISTORY_SIZE = 100;
    float fpsHistory[FPS_HISTORY_SIZE] = { 0 };
    int historyOffset = 0;

    while (!glfwWindowShouldClose(window))
	{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fpstime += deltaTime;
        cnt++;

        if(fpstime >= 1.0f) {
            // std::cout << "FPS: " << cnt << std::endl;
            fpstime = 0.0f;
            fps_count = cnt;
            cnt = 0;

            // Update history buffer
            fpsHistory[historyOffset] = (float)fps_count;
            historyOffset = (historyOffset + 1) % FPS_HISTORY_SIZE;
        }

		glClearColor(0.9f, 0.9f, 0.98f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ui.NewFrame();

        shaderProgram.use();
        glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f);

		vao.Bind();
        elements.drawGrid(shaderProgram, offsetLoc, camMatrixLoc, colorLoc);
        vao.Unbind();

        vao2.Bind();
        elements.drawCone(shaderProgram, offsetLoc, camMatrixLoc, colorLoc);
        vao2.Unbind();

        vao3.Bind();
        elements.drawCar(shaderProgram, offsetLoc, camMatrixLoc, colorLoc);
        vao3.Unbind();

        ui.Show(fps_count, size, fpsHistory, FPS_HISTORY_SIZE, historyOffset);
        ui.Render();


        camera.Inputs(window, deltaTime, elements);  
		camera.Matrix(90.0f, 0.01f, 500.0f, shaderProgram, camMatrixLoc);

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

    ui.Cleanup();
    glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}
