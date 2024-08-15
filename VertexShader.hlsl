cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
    float4 color : COLOR0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
    float4 color : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.pos = mul(input.pos, worldViewProj);
    output.coord = input.coord;
    output.norm = input.norm;
    output.color = input.color;
    return output;
}