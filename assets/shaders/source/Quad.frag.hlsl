Texture2DArray<float4> u_Textures : register(t0, space2);
SamplerState u_Sampler : register(s0, space2);

struct Input
{
    float4 Color : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float TilingFactor : TEXCOORD2;
};

void main(Input input, nointerpolation float v_TexIndex : TEXCOORD3, nointerpolation int v_EntityID : TEXCOORD4, out float4 color : SV_Target0, out int entityID : SV_Target1)
{
    float2 uv = input.TexCoord * input.TilingFactor;
    float layer = v_TexIndex;

    float4 texColor = u_Textures.Sample(u_Sampler, float3(uv, layer));
    texColor *= input.Color;

    if (texColor.a == 0.0)
        discard;

    color = texColor;
    entityID = v_EntityID;
}
