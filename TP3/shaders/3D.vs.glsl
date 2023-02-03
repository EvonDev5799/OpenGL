#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec3 aVertexTexCoord;

uniform mat4 uMVPMatrix, uMVMatrix et uNormalMatrix;

out vec2 vViewVertexNormal;
out vec2 vViewVertexPosition;
out vec2 vUV;

out vec2 vVertexPosition;
out vec2 vVertexNormal;
out vec2 vVertexTexCoord;

void main() {
    vVertexTexCoord = aVertexTexCoord;
    vVertexPosition = uMVMatrix * aVertexPosition
};