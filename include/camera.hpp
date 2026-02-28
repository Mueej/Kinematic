#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shader.hpp"
#include"elements.hpp"

class Camera {
public:
	glm::vec3 Position;
    glm::vec3 target = glm::vec3(0.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = 0.0f;
    float pitch = 0.0f;
    float radius = 10.0f;
    float speed;

	bool firstClick = true;

	int width;
	int height;

    float sensitivity = 1000.0f;
    float zoomSpeed = 10.0f;

	Camera(int width, int height, float radius);

	void UpdatePosition();
    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, int uniform);
    void Inputs(GLFWwindow* window, float deltatime, Elements& elements);
    void ScrollZoom(double yoffset, bool shiftPressed = true);
};
#endif