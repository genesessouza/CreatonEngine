#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_LightSpaceMatrix;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec4 v_FragPosLightSpace;

void main()
{
    vec4 worldPos = u_ModelMatrix * vec4(a_Position, 1.0);

    v_FragPos = vec3(worldPos);
    
    v_Normal = normalize(u_NormalMatrix * a_Normal);

    v_FragPosLightSpace = u_LightSpaceMatrix * worldPos;
    
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
in vec4 v_FragPosLightSpace;
out vec4 fragColor;

uniform vec4 u_Color; // Material color
uniform bool u_HasDirLight;
uniform vec3 u_DirLightDir;
uniform vec4 u_DirLightColor;
uniform float u_LightIntensity;

uniform int u_PointLightCount;
uniform PointLight u_PointLights[8];

uniform vec3 u_ViewPos;
uniform float u_AmbientStrength = 0.1;
uniform float u_DiffuseStrength = 0.5;
uniform float u_SpecularStrength = 1.0;

uniform float u_Shininess = 16.0;
uniform vec4 u_SpecularColor;

uniform sampler2D u_ShadowMap;

uniform int u_RenderMode;

uniform int u_ShadowType;           // 0: Off, 1: Hard, 2: PCF, 3: PCSS
uniform float u_ShadowBias;
uniform float u_ShadowStrength;     // 0.0 - 1.0

/*
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* ------------------ FUCKEN XTREME PCSS Shadows ---------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
*/
#define BLOCKER_SAMPLES 16
#define PCF_SAMPLES 16
#define NEAR_PLANE 0.1

uniform float u_LightSize;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.65433925, -0.006833270 ), vec2( -0.16533519, 0.99641472 ), 
   vec2( 0.60771394, 0.51228144 ), vec2( -0.43011155, -0.42876837 ), 
   vec2( 0.17643479, -0.58410221 ), vec2( 0.13705973, 0.71085177 ) 
);

float FindBlocker(vec2 uv, float zReceiver, vec2 texelSize) {
    float blockerSum = 0.0;
    int numBlockers = 0;
    
    float searchRegion = u_LightSize * texelSize.x * 20.0; 

    for(int i = 0; i < BLOCKER_SAMPLES; i++) {
        float d = texture(u_ShadowMap, uv + poissonDisk[i] * searchRegion).r;
        if(d < zReceiver - 0.001) {
            blockerSum += d;
            numBlockers++;
        }
    }
    return numBlockers > 0 ? blockerSum / float(numBlockers) : -1.0;
}

float ShadowCalculation_PCSS(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0) return 0.0;

    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    float zReceiver = projCoords.z;

    float avgBlockerDepth = FindBlocker(projCoords.xy, zReceiver, texelSize);
    
    if(avgBlockerDepth == -1.0) return 0.0;

    float penumbraSize = (zReceiver - avgBlockerDepth) * u_LightSize / avgBlockerDepth;

    float shadow = 0.0;
    float filterRadius = clamp(penumbraSize, texelSize.x, texelSize.x * 15.0); 

    for(int i = 0; i < PCF_SAMPLES; i++) {
        float pcfDepth = texture(u_ShadowMap, projCoords.xy + poissonDisk[i] * filterRadius).r;
        if(zReceiver - 0.002 > pcfDepth) shadow += 1.0;
    }

    return shadow / float(PCF_SAMPLES);
}

/*
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* --------------------- Basic PCF with noise ------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
*/

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float ShadowCalculation_PCF(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(-u_DirLightDir);

    float shadow = 0.0;
    
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    float randomAngle = rand(v_FragPos.xy) * 6.2831;

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float noiseX = rand(projCoords.xy + vec2(x, y)) * 0.5;
            float noiseY = rand(projCoords.yx + vec2(x, y)) * 0.5;
            vec2 offset = (vec2(x, y) + vec2(noiseX, noiseY)) * texelSize;
            
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + offset).r; 
            shadow += currentDepth - u_ShadowBias > pcfDepth ? 1.0 : 0.0;    
        }    
    }
    
    shadow /= 9.0;

    return shadow;
}

/*
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* --------------------- Basic Hard Shadows --------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
* -------------------------------------------------------------------
*/
float ShadowCalculation_Hard(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(-u_DirLightDir);

    float shadow = currentDepth - u_ShadowBias > closestDepth  ? 1.0 : 0.0;

    if(projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

void main()
{
    vec3 norm = normalize(v_Normal);    
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    
    vec3 totalAmbient = u_AmbientStrength * u_Color.rgb;
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    if (u_HasDirLight) {
        vec3 lightDir = normalize(-u_DirLightDir);
        
        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += u_DiffuseStrength * diff * u_DirLightColor.rgb * u_LightIntensity;

	    if(diff > 0.0f)
	    {
            // Specular (Blinn-Phong)
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Shininess);
            totalSpecular += u_SpecularStrength * spec * u_DirLightColor.rgb * u_LightIntensity;
	    }
    }

    for (int i = 0; i < u_PointLightCount; i++) {
        vec3 lightDir = normalize(u_PointLights[i].Position - v_FragPos);
        
        float dist = length(u_PointLights[i].Position - v_FragPos);
        float attenuation = 1.0 / (dist * dist + 1.0);

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += u_DiffuseStrength * diff * u_PointLights[i].Color.rgb * u_PointLights[i].Intensity * attenuation;

	    if(diff > 0.0f)
	    {
            // Specular (Blinn-Phong)
            vec3 halfwayDir = normalize(lightDir + viewDir);            
            float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Shininess);
	        totalSpecular += u_SpecularStrength * spec * u_PointLights[i].Color.rgb * u_PointLights[i].Intensity * attenuation;
	    }
    }

    // Ambient
    if(u_RenderMode == 0) {
        fragColor = vec4(totalAmbient, 1.0);
        return;
    }

    // Diffuse
    if(u_RenderMode == 1) {
        fragColor = vec4(totalDiffuse, 1.0);
        return;
    }

    // Specular
    if(u_RenderMode == 2) {
        fragColor = vec4(totalSpecular, 1.0);
        return;
    }

    // Normals
    if (u_RenderMode == 3) {
        fragColor = vec4(normalize(v_Normal) * 0.5 + 0.5, 1.0);
        return;
    }

    // Depth
    if (u_RenderMode == 4) {
        vec3 projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;

        // If projCoords is out of [0,1], frag is out of light range
        if(projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
            fragColor = vec4(0.2, 0.2, 0.2, 1.0); // dark grey to out of bounds
        } else {
            float depth = texture(u_ShadowMap, projCoords.xy).r;
            fragColor = vec4(vec3(depth), 1.0);
        }
        return;
    }
    
    // ShadowType: 0 = No Shadow
    float shadow = 0.0;

    if (u_ShadowType == 1) {
        shadow = ShadowCalculation_Hard(v_FragPosLightSpace);
    } 
    else if (u_ShadowType == 2) {
        shadow = ShadowCalculation_PCF(v_FragPosLightSpace); 
    } 
    else if (u_ShadowType == 3) {
        shadow = ShadowCalculation_PCSS(v_FragPosLightSpace);
    }

    shadow *= u_ShadowStrength;

    vec3 lighting = totalAmbient + (1.0 - shadow) * ((totalDiffuse * u_Color.rgb) + totalSpecular);

    // Lit
    fragColor = vec4(lighting, u_Color.a);
}