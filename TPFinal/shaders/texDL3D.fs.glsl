#version 330 core

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform sampler2D uTexture;

// Sorties du shader
in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fFragColor;

vec3 blinnPhong() {
    return uLightIntensity * (uKd * (dot(normalize(uLightDir_vs),vNormal_vs)) + uKs * pow(dot(((normalize(-vPosition_vs)+uLightIntensity)/2), vNormal_vs), uShininess));
}

void main() {
    fFragColor = texture(uTexture, vTexCoords).xyz * blinnPhong();
}