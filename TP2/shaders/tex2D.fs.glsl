#version 330 core

in vec2 vUV;

out vec3 fFragColor;

uniform sampler2D uTexture;

void main() {
  fFragColor = texture(uTexture, vUV).xyz;
};