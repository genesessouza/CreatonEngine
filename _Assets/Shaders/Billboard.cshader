#shader vertex
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec3 u_LightPosition;
uniform vec2 u_Size;

void main() {
    // Pega a posição da luz no espaço da câmera
    vec4 viewPos = u_View * vec4(u_LightPosition, 1.0);
    
    // Aplica o offset do quad diretamente no espaço da câmera (anulando rotação)
    vec3 pos = a_Position; 
    viewPos.xy += pos.xy * u_Size;

    gl_Position = u_Projection * viewPos;
}