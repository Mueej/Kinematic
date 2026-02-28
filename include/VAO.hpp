#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.hpp"

class VAO
{
public:
	GLuint ID;

	VAO();
    ~VAO();

	void LinkVBO(VBO& VBO, GLuint layout, int size);
	void Bind();
	void Unbind();
};

#endif
