#shader vertex
#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_HDRBuffer;
uniform float u_Exposure = 1.0;

void main()
{
    vec3 hdrColor = texture(u_HDRBuffer, TexCoords).rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * u_Exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}