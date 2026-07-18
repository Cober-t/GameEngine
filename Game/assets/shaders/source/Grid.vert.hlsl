cbuffer Camera : register(b0)
{
    float4x4 u_Projection;
    float4x4 u_View;
};

struct Input
{
    float3 Position : TEXCOORD0;
    float3 CameraPosition : TEXCOORD1;
    int Game2D : TEXCOORD2;
    float4 PatternSizes : TEXCOORD3;
    float PatternNumber : TEXCOORD4;
    float Opacity : TEXCOORD5;
};

struct Output
{
    float3 CameraPosition : TEXCOORD0;
    float4 WorldPoint : TEXCOORD1;
    float4 WorldCamera : TEXCOORD2;
    int Game2D : TEXCOORD3;
    float4 PatternSizes : TEXCOORD4;
    float PatternNumber : TEXCOORD5;
    float Opacity : TEXCOORD6;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;

    float4 point4 = mul(u_Projection, mul(u_View, float4(input.Position, 1.0f)));
    output.Position = point4;

    output.CameraPosition = input.CameraPosition;
    output.WorldPoint = float4(input.Position, 1.0f);
    output.WorldCamera = point4;
    output.Game2D = input.Game2D;
    output.PatternSizes = input.PatternSizes;
    output.PatternNumber = input.PatternNumber;
    output.Opacity = input.Opacity;

    return output;
}
