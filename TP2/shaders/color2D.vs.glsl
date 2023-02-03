#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vVertexPosition;
out vec2 vVertexCenter;

mat3 translate(float tx, float ty)
{
  return mat3(vec3( 1,0,0),vec3(0,1,0),vec3(tx,ty,1));
}

mat3 scale(float sx, float sy)
{
  return mat3(vec3(sx,0,0),vec3(0,sy,0),vec3(0,0,1));
}

mat3 rotate(float a)
{
  float angle = radians(a);
  return mat3(vec3(cos(angle),sin(angle),0),vec3(-sin(angle),cos(angle),0),vec3(0,0,1));
}

void main() {
  mat3 transform = translate(0.5f, 0.5f);
  vec2 pos = (transform * vec3(aVertexPosition,1)).xy;
  vVertexCenter = (transform * vec3(vec2(0,0),1)).xy;
  vVertexPosition = pos;
  vFragColor = aVertexColor;
  gl_Position = vec4(pos, 0, 1);
};