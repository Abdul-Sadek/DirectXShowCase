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
{
   // Sample the texture
    float4 texColor = Texture.Sample(TextureSampler, input.coord);

    // Normalize the normal vector from the PS_INPUT structure
    float3 normal = normalize(input.norm);

    // Calculate the light direction (assuming directional or point light)
    float3 lightDir = normalize(light.Position - input.pos.xyz);

    // Diffuse lighting calculation
    float diff = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = diff * light.Color * light.Intensity;

    // Specular lighting calculation (optional)
    float3 viewDir = normalize(cameraPos - input.pos.xyz);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16); // Shininess is 16
    float3 specular = spec * light.Color * light.Intensity;

    // Combine texture color with diffuse and specular lighting
    float3 finalColor = (texColor.rgb * diffuse) + specular;

    // Optionally mix with vertex color if needed
    //finalColor *= input.color.rgb;

    // Return the final color with the texture's alpha value
    return float4(finalColor, texColor.a);
}