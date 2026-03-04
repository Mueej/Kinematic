#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "imgui.h"

Camera::Camera(int w, int h, float radius_) {
    width = w;
    height = h;
    radius = radius_;
    speed = 5.0f;
    UpdatePosition();
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, int uniform) {
    shader.use();
    glm::mat4 view = glm::lookAt(Position, target, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::UpdatePosition() {
    float radYaw   = glm::radians(yaw);
    float radPitch = glm::radians(pitch);

    float x = radius * cos(radPitch) * cos(radYaw);
    float y = radius * sin(radPitch);
    float z = radius * cos(radPitch) * sin(radYaw);

    Position = target + glm::vec3(x, y, z);
}

void Camera::Inputs(GLFWwindow* window, float deltatime, Elements& elements) {
    static double lastX = width / 2.0;
    static double lastY = height / 2.0;
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    width = w;
    height = h;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        elements.posX += speed * deltatime;
        elements.posGX += speed * deltatime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        elements.posX -= speed * deltatime;
        elements.posGX -= speed * deltatime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        elements.posZ -= speed * deltatime;
        elements.posGZ -= speed * deltatime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        elements.posZ += speed * deltatime;
        elements.posGZ += speed * deltatime;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (ImGui::GetIO().WantCaptureMouse) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (firstClick) {
                lastX = mouseX;
                lastY = mouseY;
                firstClick = false;
            }

            float deltaX = static_cast<float>(mouseX - lastX);
            float deltaY = static_cast<float>(lastY - mouseY);

            lastX = mouseX;
            lastY = mouseY;

            yaw   += deltaX * sensitivity * 0.01f * deltatime;
            pitch -= deltaY * sensitivity * 0.01f * deltatime;

            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
    } 
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }

    UpdatePosition();
}

void Camera::ScrollZoom(double yoffset, bool shiftPressed) {
    radius -= static_cast<float>(yoffset) * zoomSpeed;

    if (radius < 1.0f)  radius = 1.0f;
    if (radius > 100.0f) radius = 100.0f;

    UpdatePosition();
}