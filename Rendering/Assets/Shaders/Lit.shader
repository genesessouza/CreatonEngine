#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_ViewProjectionMatrix;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    vec4 worldPos = u_ModelMatrix * vec4(a_Position, 1.0);

    v_FragPos = vec3(worldPos);
    
    v_Normal = normalize(u_NormalMatrix * a_Normal);

    gl_Position = u_ViewProjectionMatrix * worldPos;
};


#shader fragment
#version 450 core

precision highp float;

struct PointLight {
    vec3 Position;
    vec4 Color;
    float Intensity;
};

in vec3 v_Normal;
in vec3 v_FragPos;
out vec4 fragColor;

uniform vec4 u_Color; // Material color
uniform bool u_HasDirLight;
uniform vec3 u_DirLightDir;
uniform vec4 u_DirLightColor;
uniform float u_LightIntensity;

uniform int u_PointLightCount;
uniform PointLight u_PointLights[8];

uniform vec3 u_ViewPos;
uniform float u_DiffuseStrength = 0.5;
uniform float u_SpecularStrength = 1.0;
uniform float u_Shininess = 16.0;

void main()
{
    vec3 norm = normalize(v_Normal);    
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    
    vec3 ambient = 0.1 * u_Color.rgb;
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    if (u_HasDirLight) {
        vec3 lightDir = normalize(u_DirLightDir);
        
        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += u_DiffuseStrength * diff * u_DirLightColor.rgb * u_LightIntensity;

	    if(diff != 0.0f)
	    {
            // Specular (Blinn-Phong)
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Shininess);
            totalSpecular += u_SpecularStrength * spec * u_DirLightColor.rgb;
	    }
    }

    for (int i = 0; i < u_PointLightCount; i++) {
        vec3 lightDir = normalize(u_PointLights[i].Position - v_FragPos);
        
        float dist = length(u_PointLights[i].Position - v_FragPos);
        float attenuation = u_PointLights[i].Intensity / (dist + 1.0);

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += u_DiffuseStrength * diff * u_PointLights[i].Color.rgb * attenuation;

	    if(diff != 0.0f)
	    {
            // Specular (Blinn-Phong)
            vec3 halfwayDir = normalize(lightDir + viewDir);            
            float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Shininess);
	        totalSpecular += u_SpecularStrength * spec * u_PointLights[i].Color.rgb * attenuation;
	    }
    }

    vec3 result = (ambient + totalDiffuse) * u_Color.rgb + totalSpecular;    
    result = pow(result, vec3(1.0 / 2.2));
    // fragColor = vec4(normalize(v_Normal) * 0.5 + 0.5, 1.0);
    fragColor = vec4(result, u_Color.a);
}