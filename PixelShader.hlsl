struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

PSOutput main(PS_INPUT input)
{
    PSOutput output = (PSOutput) 0;
    output.color = input.color;
    return output;
}