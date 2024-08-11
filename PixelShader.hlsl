struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 coord : TEXCOORD0;
    float3 norm : NORMAL0;
};

struct PSOutput
{
    float4 norm : SV_Target;
};

PSOutput main(PS_INPUT input)
{
    PSOutput output;
    output.norm = float4(input.norm, 1.0f);
    return output;
}