#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<vector>
#include <tuple>

#include <string>

#include"shader.hpp"

class Elements {
public:
    float axes[12];
    std::vector<float> coneSTL;
    std::vector<float> carSTL;

    GLfloat lightVertices[24] =
    { 
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
        0.1f,  0.1f, -0.1f,
        0.1f,  0.1f,  0.1f
    };

    GLuint lightIndices[36] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    float carWidth = 2.0f, carHeight = 2.0f, carLength = 3.0f;
    
    float length;
    float height;
    float width;
    int num;
    float diff;

    float posX = 0.0f, posZ = 0.0f;
    float posGX = 0.0f, posGZ = 0.0f;

    std::vector<std::tuple<float, float, float, char>> coneCoordinates;

    glm::vec3 carScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 carRotation = glm::vec3(0.0f, 0.0f, 0.0f); // Euler angles (x, y, z)
    glm::vec3 carPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 coneScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 coneRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 conePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    Elements(int num, float height, float width, float diff, const std::string& conePath, const std::string& carPath);
    void drawGrid(Shader& shader, int offsetLoc, int modelLoc, int colorLoc);
    void drawCone(Shader& shader, int offsetLoc, int modelLoc, int colorLoc);
    void drawCar(Shader& shader, int offsetLoc, int modelLoc, int colorLoc);
    void drawLight(Shader& shader, int modelLoc, int colorLoc, glm::vec3 lightPos);
    
    std::vector<float> loadSTL(const std::string& filepath);
    void clamp();
};

#endif