cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.pos = mul(input.pos, worldViewProj);
    output.coord = input.coord;
    output.norm = input.norm;
    return output;
}