struct Input
{
    float3 LocalPosition : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float Thickness : TEXCOORD2;
    float Fade : TEXCOORD3;
    int EntityID : TEXCOORD4;
};

struct Output
{
    float4 Color : SV_Target0;
    int EntityID : SV_Target1;
};

Output main(Input input)
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(input.LocalPosition);
    float circle = smoothstep(0.0, input.Fade, distance);
    circle *= smoothstep(input.Thickness + input.Fade, input.Thickness, distance);

    if (circle == 0.0)
        discard;

    Output output;
    output.Color = input.Color;
    output.Color.a *= circle;
    output.EntityID = input.EntityID;
    return output;
}
