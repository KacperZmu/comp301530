#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D BaseTex;
layout (binding = 1) uniform sampler2D AlphaTex;
layout (binding = 2) uniform samplerCube SkyBoxTex;

uniform struct LightInfo {
    vec4 Position; 
    vec3 La;
    vec3 L;
} Light;

uniform struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
} Material;

uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    vec3 Color;
} Fog;

const int levels = 9;
const float scaleFactor = 1.0 / levels;

vec3 ToonPhong(vec3 position, vec3 n) {
    vec3 diffuse = vec3(0), spec = vec3(0);
    vec3 texColor = texture(BaseTex, TexCoord).rgb;
    vec3 ambient = Light.La * texColor;
    vec3 s = normalize(Light.Position.xyz - position);
    float sDotN = max(dot(s, n), 0.0);
    diffuse = texColor * floor(sDotN * levels) * scaleFactor;
    if (sDotN > 0.0) {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(v + s);
        spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess); 
    }
    return ambient + (diffuse + spec) * Light.L * -1.5;
}

void main() {
    vec3 textColor = texture(SkyBoxTex, normalize(Vec)).rgb;
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    vec3 shaderColor = ToonPhong(Position, normalize(Normal));
    vec3 color = mix(Fog.Color, shaderColor, fogFactor);
    vec4 finalColor = vec4(color, 1.0) + vec4(ToonPhong(Position, normalize(-Normal)), 1.0) + vec4(textColor, 1.0);
    FragColor = finalColor;

}

