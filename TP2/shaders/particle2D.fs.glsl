#version 330 core

in vec3 vFragColor;
in vec2 vFragPos;

out vec3 fFragColor;

void main() {
  float factor = 0.1f * exp(0.1f * distance(gl_FragCoord.xy, vFragPos) * distance(gl_FragCoord.xy, vFragPos));
  fFragColor = factor * vFragColor;
};