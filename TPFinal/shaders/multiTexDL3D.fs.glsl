#version 330 core

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

// Sorties du shader
in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fFragColor;

vec3 blinnPhong() {
    return uLightIntensity * (uKd * (dot(normalize(uLightDir_vs),normalize(vNormal_vs))) + uKs * pow(dot(((normalize(-vPosition_vs)+normalize(uLightDir_vs))/2), normalize(vNormal_vs)), uShininess));
}

void main() {
    fFragColor = texture(uTexture0, vTexCoords).xyz * texture(uTexture1, vTexCoords).xyz * blinnPhong();
}