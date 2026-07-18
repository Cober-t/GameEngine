Texture2D<float4> u_Textures[32] : register(t0);
SamplerState u_Sampler : register(s0);

struct Input
{
    float4 Color : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float TilingFactor : TEXCOORD2;
};

void main(Input input, nointerpolation float v_TexIndex : TEXCOORD3, nointerpolation int v_EntityID : TEXCOORD4, out float4 color : SV_Target0, out int entityID : SV_Target1)
{
    float4 texColor = input.Color;

    int idx = (int)v_TexIndex;
    if (idx == 0)       texColor *= u_Textures[ 0].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 1)  texColor *= u_Textures[ 1].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 2)  texColor *= u_Textures[ 2].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 3)  texColor *= u_Textures[ 3].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 4)  texColor *= u_Textures[ 4].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 5)  texColor *= u_Textures[ 5].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 6)  texColor *= u_Textures[ 6].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 7)  texColor *= u_Textures[ 7].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 8)  texColor *= u_Textures[ 8].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 9)  texColor *= u_Textures[ 9].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 10) texColor *= u_Textures[10].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 11) texColor *= u_Textures[11].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 12) texColor *= u_Textures[12].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 13) texColor *= u_Textures[13].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 14) texColor *= u_Textures[14].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 15) texColor *= u_Textures[15].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 16) texColor *= u_Textures[16].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 17) texColor *= u_Textures[17].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 18) texColor *= u_Textures[18].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 19) texColor *= u_Textures[19].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 20) texColor *= u_Textures[20].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 21) texColor *= u_Textures[21].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 22) texColor *= u_Textures[22].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 23) texColor *= u_Textures[23].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 24) texColor *= u_Textures[24].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 25) texColor *= u_Textures[25].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 26) texColor *= u_Textures[26].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 27) texColor *= u_Textures[27].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 28) texColor *= u_Textures[28].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 29) texColor *= u_Textures[29].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 30) texColor *= u_Textures[30].Sample(u_Sampler, input.TexCoord * input.TilingFactor);
    else if (idx == 31) texColor *= u_Textures[31].Sample(u_Sampler, input.TexCoord * input.TilingFactor);

    if (texColor.a == 0.0)
        discard;

    color = texColor;
    entityID = v_EntityID;
}
