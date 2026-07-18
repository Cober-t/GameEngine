cbuffer Camera : register(b0)
{
    float4x4 u_Projection;
    float4x4 u_View;
};

struct Input
{
    float3 WorldPosition : TEXCOORD0;
    float3 LocalPosition : TEXCOORD1;
    float4 Color : TEXCOORD2;
    float Thickness : TEXCOORD3;
    float Fade : TEXCOORD4;
    int EntityID : TEXCOORD5;
};

struct Output
{
    float3 LocalPosition : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float Thickness : TEXCOORD2;
    float Fade : TEXCOORD3;
    int EntityID : TEXCOORD4;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.LocalPosition = input.LocalPosition;
    output.Color = input.Color;
    output.Thickness = input.Thickness;
    output.Fade = input.Fade;
    output.EntityID = input.EntityID;

    float4 worldPos = float4(input.WorldPosition, 1.0f);
    output.Position = mul(u_Projection, mul(u_View, worldPos));

    return output;
}
