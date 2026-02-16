#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_LightSpaceMatrix;

void main()
{
    gl_Position = u_LightSpaceMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}

#shader fragment
#version 450 core

void main()
{

}