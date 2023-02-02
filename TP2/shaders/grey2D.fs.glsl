#version 330 core

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  float value = vFragColor.x/3 + vFragColor.y/3 + vFragColor.z/3;
  fFragColor = vec3(value, value, value);
};