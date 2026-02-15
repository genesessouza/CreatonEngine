#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Offset;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ViewProjectionMatrix;
uniform vec3 u_Position;
uniform float u_Size;

void main()
{
	vec3 camRight = vec3(u_ViewMatrix[0][0], u_ViewMatrix[1][0], u_ViewMatrix[2][0]);
    vec3 camUp    = vec3(u_ViewMatrix[0][1], u_ViewMatrix[1][1], u_ViewMatrix[2][1]);

    vec2 offset = a_Offset.xy;

    vec3 worldPos = u_Position 
        + camRight * offset.x * u_Size
        + camUp    * offset.y * u_Size;

	gl_Position = u_ViewProjectionMatrix * vec4(worldPos, 1.0);
}

#shader fragment
#version 450 core

uniform vec4 u_Color;
out vec4 FragColor;

void main()
{
	FragColor = u_Color;
}