#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aUV;

uniform mat3 uModelMatrix;

out vec2 vUV;

mat3 rotate(float a)
{
  float angle = radians(a);
  return mat3(vec3(cos(angle),sin(angle),0),vec3(-sin(angle),cos(angle),0),vec3(0,0,1));
}

void main() {
    vUV = aUV;
    vec2 transformed = (uModelMatrix * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(transformed, 0, 1);
};