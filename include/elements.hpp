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

#include"shader.hpp"

class Elements {
public:
    float axes[12];
    float cone[24];
    float car[24];

    GLuint Indices[36] = {
        0, 1, 2, 
        0, 2, 3, 
        4, 5, 6, 
        4, 6, 7,
        0, 4, 7,
        0, 3, 7, 
        3, 2, 7,
        7, 6, 2,
        1, 5, 6, 
        1, 2, 6, 
        0, 5, 1, 
        0, 4, 5
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

    Elements(int num, float height, float width, float diff);
    void drawGrid(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc);
    void drawCone(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc);
    void drawCar(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc);
    void clamp();
    
};

#endif