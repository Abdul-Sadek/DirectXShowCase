SamplerState TextureSampler : register(s0);
Texture2D<float4> Texture : register(t0);

cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj;
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
    return Texture.Sample(TextureSampler, input.coord);
}
