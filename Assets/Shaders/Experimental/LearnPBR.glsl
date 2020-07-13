#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

#include "Scene/BRDF.glsl"

#define LIGHT_COUNT 4
#define INSTANCE_COUNT 49

layout(binding = 0) uniform FrameUB
{
    mat4 viewProj;
    vec3 lightPositions[LIGHT_COUNT];
    vec3 lightColors[LIGHT_COUNT];
    vec3 camPos;
};

struct ObjectData
{
    mat4 model;
    vec3 albedo;
    float ao;
    float roughness;
    float metallic; 
};

layout(binding = 1) uniform ObjectUB
{
    ObjectData objects[INSTANCE_COUNT];
};

#type vertex

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

layout(location = 0) out vec3 WorldPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out flat int InstanceIndex;

void main() 
{
    mat4 model = objects[gl_InstanceIndex].model;

    WorldPos = vec3(model * vec4(VertexPosition, 1.0));

    gl_Position = viewProj * vec4(WorldPos, 1.0);
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

    Normal = mat3(model) * VertexNormal; // no inv transpose

    InstanceIndex = gl_InstanceIndex;
}


#type pixel

layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in flat int InstanceIndex;

layout(location = 0) out vec4 FragmentColor;

void main() 
{
    ObjectData data = objects[InstanceIndex];
    vec3 albedo = data.albedo;
    float ao = data.ao;
    float metallic = data.metallic;
    float roughness = data.roughness;

    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);

        float dist = length(lightPositions[i] - WorldPos);
        float atten = 1.0 / (dist * dist);
        vec3 Li = lightColors[i] * atten;

        float NdotH = max(0.0, dot(N, H));
        float NdotL = max(0.0, dot(N, L));
        float NdotV = max(0.0, dot(N, V));
        float D = DistributionGGX(roughness, NdotH);
        float G = GeometrySmith(roughness, NdotL, NdotV);
        vec3 F = FresnelSchlick(F0, vec3(1.0), saturate(dot(H, V)));

        vec3 nominator = D * G * F; 
        float denominator = 4 * NdotV * NdotL;
        vec3 specular = nominator / max(denominator, 0.001);

        vec3 ks = F;
        vec3 kd = vec3(1.0) - ks;
        kd *= 1.0 - metallic;

        Lo += (kd * albedo / PI + specular) * Li * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    FragmentColor = vec4(color, 1.0);
}