cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProj;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = mul(input.pos, worldViewProj);
    output.color = input.color;
    return output;
}