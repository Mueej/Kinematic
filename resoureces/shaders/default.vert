#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3 offset;
uniform mat4 camMatrix;

void main()
{
   gl_Position = camMatrix * vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z + offset.z, 1.0);
}