#version 330 core

uniform sampler2D uTexture;

// Sorties du shader
in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fFragColor;

void main() {
    fFragColor = texture(uTexture, vTexCoords).xyz;
}