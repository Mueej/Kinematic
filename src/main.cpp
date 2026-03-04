#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "elements.hpp"
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "camera.hpp"
#include "EBO.hpp"
#include "ui.hpp"

int main() {
    Elements elements(100, 1.5f, 0.5f, 2.0f, "../resoureces/models/cone.stl", "../resoureces/models/car.stl");

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
    Shader lightShader("../resoureces/shaders/light.vert", "../resoureces/shaders/light.frag");

    VAO lightVAO;
	lightVAO.Bind();
	VBO lightVBO(elements.lightVertices, sizeof(elements.lightVertices));
	EBO lightEBO(elements.lightIndices, sizeof(elements.lightIndices));
	lightVAO.LinkVBO(lightVBO, 0, 3);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

    VAO vao;
    vao.Bind();
    VBO vbo(elements.axes, sizeof(elements.axes));
    vao.LinkVBO(vbo, 0, 3);
    vao.Unbind();
    vbo.Unbind();

    VAO vao2;
    vao2.Bind();
    VBO vbo2(elements.coneSTL.data(), elements.coneSTL.size() * sizeof(float));
    vao2.LinkAttrib(vbo2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao2.LinkAttrib(vbo2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao2.Unbind();
    vbo2.Unbind();

    VAO vao3;
    vao3.Bind();
    VBO vbo3(elements.carSTL.data(), elements.carSTL.size() * sizeof(float));
    vao3.LinkAttrib(vbo3, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao3.LinkAttrib(vbo3, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao3.Unbind();
    vbo3.Unbind();

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
    int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    
    int isLitLoc = glGetUniformLocation(shaderProgram.ID, "isLit");
    int lightPosLoc = glGetUniformLocation(shaderProgram.ID, "lightPos");
    int meshLightColorLoc = glGetUniformLocation(shaderProgram.ID, "lightColor");
    int viewPosLoc = glGetUniformLocation(shaderProgram.ID, "viewPos");

    int lightColorLoc = glGetUniformLocation(lightShader.ID, "lightColor");
    int lightCamMatrixLoc = glGetUniformLocation(lightShader.ID, "camMatrix");
    int lightModelLoc = glGetUniformLocation(lightShader.ID, "model");

    glm::vec3 baseLightPos(50.0f, 50.0f, 50.0f);

    while (!glfwWindowShouldClose(window))
	{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float speed = 60.0f + 20.0f * sin(currentFrame);
        float accX = 20.0f * cos(currentFrame * 2.0f);
        float accY = 10.0f * sin(currentFrame * 1.5f);
        float charge = 100.0f - fmod(currentFrame * 5.0f, 100.0f);
        
        ui.UpdateTelemetry(speed, accX, accY, charge);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ui.NewFrame();

        shaderProgram.use();
        glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f);
        
        glUniform3f(lightPosLoc, baseLightPos.x, baseLightPos.y, baseLightPos.z);
        glUniform3f(meshLightColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		vao.Bind();
        glUniform1i(isLitLoc, 0); 
        elements.drawGrid(shaderProgram, offsetLoc, modelLoc, colorLoc);
        vao.Unbind();

        glUniform1i(isLitLoc, 1);

        vao2.Bind();
        elements.drawCone(shaderProgram, offsetLoc, modelLoc, colorLoc);
        vao2.Unbind();

        vao3.Bind();
        elements.drawCar(shaderProgram, offsetLoc, modelLoc, colorLoc);
        vao3.Unbind();

        lightShader.use();
        camera.Matrix(90.0f, 0.01f, 500.0f, lightShader, lightCamMatrixLoc);
        lightVAO.Bind();
        elements.drawLight(lightShader, lightModelLoc, lightColorLoc, baseLightPos);
        lightVAO.Unbind();

        ui.Show();
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
