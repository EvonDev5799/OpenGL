#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;
in vec2 vVertexCenter;

out vec3 fFragColor;

void main() {
  float d = distance(vVertexPosition, vVertexCenter);
  float coeff = smoothstep(0.3, 0.32, length(fract(5.0 * vVertexPosition) - 0.5));
  //float coeff =5 * exp(-100 * distance(vVertexPosition,vVertexCenter)*distance(vVertexPosition,vVertexCenter));
  fFragColor = vFragColor * coeff;
  // float value = fFragColor.x/3 + fFragColor.y/3 + fFragColor.z/3;
  // fFragColor = vec3(value, value, value);
};