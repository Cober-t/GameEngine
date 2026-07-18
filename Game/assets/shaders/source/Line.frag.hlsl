struct Input
{
    float4 Color : TEXCOORD0;
    int EntityID : TEXCOORD1;
};

struct Output
{
    float4 Color : SV_Target0;
    int EntityID : SV_Target1;
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    output.EntityID = input.EntityID;
    return output;
}
