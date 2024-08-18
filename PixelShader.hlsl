Texture2D<float4> Texture : register(t0);
SamplerState TextureSampler : register(s0);

struct Light
{
    float3 Position;
    float3 Direction;
    float3 Color;
    float Intensity;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj; // Combined world-view-projection matrix
    Light light; // The light structure
    float3 cameraPos; // Camera position for specular calculations
    float padding; // Padding for alignment (to ensure 16-byte alignment)
};
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
    float4 color : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{ // Sample the texture
    float4 texColor = Texture.Sample(TextureSampler, input.coord);

    // Calculate the lighting
    float3 lightDir = light.Direction;
    float3 normal = normalize(input.norm);
    float diff = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = diff * light.Color * light.Intensity;

    // Combine the texture color with the lighting
    float3 finalColor = texColor.rgb * diffuse;

    // Return the final color with the texture's alpha value
    return float4(finalColor, texColor.a);
}