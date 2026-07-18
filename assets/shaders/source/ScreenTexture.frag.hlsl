Texture2D<float4> u_ScreenTexture : register(t0, space2);
SamplerState u_ScreenSampler : register(s0, space2);

struct Input
{
    float2 TexCoord : TEXCOORD0;
};

float4 main(Input input) : SV_Target0
{
    float4 col = u_ScreenTexture.Sample(u_ScreenSampler, input.TexCoord);
    return float4(col.rgb, 1.0);
}
