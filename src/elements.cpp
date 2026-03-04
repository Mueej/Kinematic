#include "elements.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>

Elements::Elements(int num, float height, float width, float diff, const std::string& conePath, const std::string& carPath) {
    this->length = ( 1 + num ) * diff;
    this->height = height;
    this->width = width;
    this->num = num;
    this->diff = diff;

    axes[0] = -length; axes[1] = 0.0f; axes[2] = 0.0f;
    axes[3] = length;  axes[4] = 0.0f; axes[5] = 0.0f;
    axes[6] = 0.0f;    axes[7] = 0.0f; axes[8] = length;
    axes[9] = 0.0f;    axes[10]= 0.0f; axes[11]= -length;

    coneSTL = loadSTL(conePath);
    carSTL = loadSTL(carPath);

    Config config("../config.json");
    config.Load(*this);

    coneCoordinates.push_back({-1.0f, 0.0f, -1.0f, 'b'});
    coneCoordinates.push_back({-1.0f, 0.0f, 1.0f, 'b'});
    coneCoordinates.push_back({1.0f, 0.0f, -1.0f, 'y'});
    coneCoordinates.push_back({1.0f, 0.0f, 1.0f, 'y'});
    coneCoordinates.push_back({3.0f, 0.0f, -1.0f, 'o'});
    coneCoordinates.push_back({3.0f, 0.0f, 1.0f, 'o'});
}

void Elements::drawGrid(Shader& shader, int offsetLoc, int modelLoc, int colorLoc) {
    clamp();
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    for(float i = - num * diff; i <= num * diff; i += diff) {
        glUniform3f(offsetLoc, 0.0f + posGX, 0.0f, i + posGZ);
        glDrawArrays(GL_LINES, 0, 2);
        glUniform3f(offsetLoc, i + posGX, 0.0f, 0.0f + posGZ);
        glDrawArrays(GL_LINES, 2, 2);
    }
}

void Elements::drawCone(Shader& shader, int offsetLoc, int modelLoc, int colorLoc) {
    int vertexCount = coneSTL.size() / 6; 
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, conePosition);
    model = glm::rotate(model, glm::radians(coneRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(coneRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(coneRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, coneScale);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    for(auto& t : coneCoordinates) {
        glUniform3f(offsetLoc, std::get<0>(t) + posX, std::get<1>(t) - 0.01f, std::get<2>(t) + posZ);

        if(std::get<3>(t)=='y')      glUniform3f(colorLoc, 0.9f, 0.7f, 0.0f);
        else if(std::get<3>(t)=='b') glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
        else                         glUniform3f(colorLoc, 1.0f, 0.5f, 0.0f);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
}

void Elements::drawCar(Shader& shader, int offsetLoc, int modelLoc, int colorLoc) {
    int vertexCount = carSTL.size() / 6; 

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, carPosition);
    model = glm::rotate(model, glm::radians(carRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(carRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(carRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, carScale);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(offsetLoc, 0.0f, 0.0f, 0.0f);
    glUniform3f(colorLoc, 0.0f, 0.8f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Elements::clamp() {
    if(posGX>diff) posGX -= diff;
    if(posGX<-diff) posGX += diff;
    if(posGZ>diff) posGZ -= diff;
    if(posGZ<-diff) posGZ += diff;
}

void Elements::drawLight(Shader& shader, int modelLoc, int colorLoc, glm::vec3 lightPos) {
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(lightIndices[0]), GL_UNSIGNED_INT, (void*)0);
}

std::vector<float> Elements::loadSTL(const std::string& filepath) {
    std::vector<float> vertices;
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        // std::cerr << "Failed to open STL file: " << filepath << std::endl;
        return vertices; 
    }

    file.seekg(80, std::ios::beg);

    uint32_t numTriangles;
    if (!file.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t))) {
        return vertices;
    }

    for (uint32_t i = 0; i < numTriangles; i++) {
        float normal[3];
        file.read(reinterpret_cast<char*>(normal), 3 * sizeof(float));

        for (int j = 0; j < 3; j++) {
            float vertex[3];
            file.read(reinterpret_cast<char*>(vertex), 3 * sizeof(float));

            vertices.push_back(vertex[0]);
            vertices.push_back(vertex[1]);
            vertices.push_back(vertex[2]);
            vertices.push_back(normal[0]);
            vertices.push_back(normal[1]);
            vertices.push_back(normal[2]);
        }

        uint16_t attributeByteCount;
        file.read(reinterpret_cast<char*>(&attributeByteCount), sizeof(uint16_t));
    }

    file.close();
    return vertices;
}