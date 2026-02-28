#include "elements.hpp"

Elements::Elements(int num, float height, float width, float diff) {
    this->length = ( 1 + num ) * diff;
    this->height = height;
    this->width = width;
    this->num = num;
    this->diff = diff;

    //axes
    axes[0] = -length;
    axes[1] = 0.0f;
    axes[2] = 0.0f;

    axes[3] = length;
    axes[4] = 0.0f;
    axes[5] = 0.0f;

    axes[6] = 0.0f;
    axes[7] = 0.0f;
    axes[8] = length;

    axes[9] = 0.0f;
    axes[10] = 0.0f;
    axes[11] = -length;

    //cone
    float width2 = width * 0.7;
    cone[0]  = -width; cone[1]  =   0.0f; cone[2]  = -width;
    cone[3]  = -width; cone[4]  =   0.0f; cone[5]  =  width;
    cone[6]  =  width; cone[7]  =   0.0f; cone[8]  =  width;
    cone[9]  =  width; cone[10] =   0.0f; cone[11] = -width;

    cone[12] = -width2; cone[13] = height; cone[14] = -width2;
    cone[15] = -width2; cone[16] = height; cone[17] =  width2;
    cone[18] =  width2; cone[19] = height; cone[20] =  width2;
    cone[21] =  width2; cone[22] = height; cone[23] = -width2;

    car[0]   = -carLength; car[1]  =   0.0f; car[2]  = -carWidth;
    car[3]   = -carLength; car[4]  =   0.0f; car[5]  =  carWidth;
    car[6]   =  carLength; car[7]  =   0.0f; car[8]  =  carWidth;
    car[9]   =  carLength; car[10] =   0.0f; car[11] = -carWidth;

    car[12]  = -carLength; car[13] =  carHeight * 2; car[14] = -carWidth;
    car[15]  = -carLength; car[16] =  carHeight * 2; car[17] =  carWidth;
    car[18]  =  carLength; car[19] =  carHeight * 2; car[20] =  carWidth;
    car[21]  =  carLength; car[22] =  carHeight * 2; car[23] = -carWidth;

    coneCoordinates.push_back({-1.0f, 0.0f, -1.0f, 'b'});
    coneCoordinates.push_back({-1.0f, 0.0f, 1.0f, 'b'});
    coneCoordinates.push_back({1.0f, 0.0f, -1.0f, 'y'});
    coneCoordinates.push_back({1.0f, 0.0f, 1.0f, 'y'});
    coneCoordinates.push_back({3.0f, 0.0f, -1.0f, 'o'});
    coneCoordinates.push_back({3.0f, 0.0f, 1.0f, 'o'});
}

void Elements::drawGrid(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc) {
    clamp();
    for(float i = - num * diff; i <= num * diff; i += diff) {
        glUniform3f(offsetLoc, 0.0f + posGX, 0.0f, i + posGZ);
        glDrawArrays(GL_LINES, 0, 2);
        glUniform3f(offsetLoc, i + posGX, 0.0f, 0.0f + posGZ);
        glDrawArrays(GL_LINES, 2, 2);
    }
}

void Elements::drawCone(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc) {
    for(auto& t : coneCoordinates) {
        glUniform3f(offsetLoc, std::get<0>(t) + posX, std::get<1>(t), std::get<2>(t) + posZ);

        if(std::get<3>(t)=='y')      glUniform3f(colorLoc, 0.9f, 0.7f, 0.0f);
        else if(std::get<3>(t)=='b') glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
        else                         glUniform3f(colorLoc, 1.0f, 0.5f, 0.0f);

        glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_INT, (void*)0);
    }
}

void Elements::drawCar(Shader& shader, int offsetLoc, int camMatrixLoc, int colorLoc) {
    glUniform3f(offsetLoc, 0.0f, 0.0f, 0.0f);
    glUniform3f(colorLoc, 0.0f, 0.8f, 0.8f);
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_INT, (void*)0);
}

void Elements::clamp() {
    if(posGX>diff) posGX -= diff;
    if(posGX<-diff) posGX += diff;
    if(posGZ>diff) posGZ -= diff;
    if(posGZ<-diff) posGZ += diff;
}