cbuffer Camera : register(b0)
{
    float4x4 u_Projection;
    float4x4 u_View;
};

struct Input
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float2 TexCoord : TEXCOORD2;
    float TexIndex : TEXCOORD3;
    float TilingFactor : TEXCOORD4;
    int EntityID : TEXCOORD5;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float TilingFactor : TEXCOORD2;
    float4 Position : SV_Position;
    nointerpolation float TexIndex : TEXCOORD3;
    nointerpolation int EntityID : TEXCOORD4;
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    output.TilingFactor = input.TilingFactor;
    output.TexIndex = input.TexIndex;
    output.EntityID = input.EntityID;

    float4 worldPos = float4(input.Position, 1.0f);
    output.Position = mul(u_Projection, mul(u_View, worldPos));

    return output;
}
