#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform vec3 offset;
uniform mat4 model;
uniform mat4 camMatrix;

out vec3 FragPos;
out vec3 FragNormal;

void main()
{
   vec4 localPos = model * vec4(aPos, 1.0);
   FragPos = vec3(localPos.x + offset.x, localPos.y + offset.y, localPos.z + offset.z);
   
   // We also apply the rotation (but NOT translation) part of the model matrix to the normals.
   // Note: For perfect strictness we often use `transpose(inverse(mat3(model)))` but standard `mat3(model)` works fine for uniform scales.
   FragNormal = mat3(model) * aNormal;
   
   gl_Position = camMatrix * vec4(FragPos, 1.0);
}