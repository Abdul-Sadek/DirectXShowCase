Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
};

struct PSOutput
{
    float4 color : SV_Target;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, input.coord * 0.5);
}