#include "VAO.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout, int size) {
	VBO.Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}