cbuffer Camera : register(b0, space1)
{
    float4x4 u_Projection;
    float4x4 u_View;
};

struct Input
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;

    float4 worldPos = float4(input.Position, 1.0f);
    output.Position = mul(u_Projection, mul(u_View, worldPos));

    return output;
}