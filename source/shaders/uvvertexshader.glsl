#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosMS;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out float brightness;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform float bs = 1.0;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosMS,1);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
    brightness = min(1, (bs)/(sqrt(gl_Position.x*gl_Position.x + gl_Position.y*gl_Position.y + gl_Position.z*gl_Position.z)));
}