#version 330 core

// Interpolated values from the vertex shaders
in vec3 rgb;
in float brightness;

// Ouput data
out vec3 color;

void main(){

    color = brightness * rgb;
}