struct Input
{
    float3 CameraPosition : TEXCOORD0;
    float4 WorldPoint : TEXCOORD1;
    float4 WorldCamera : TEXCOORD2;
    int Game2D : TEXCOORD3;
    float4 PatternSizes : TEXCOORD4;
    float PatternNumber : TEXCOORD5;
    float Opacity : TEXCOORD6;
};

float4 grid(float3 fragPos3D, float scale, bool drawAxis, int game2D)
{
    float2 coord;
    if (game2D != 1)
        coord = fragPos3D.xz * scale;
    else
        coord = fragPos3D.xy * scale;

    float2 derivative = fwidth(coord);
    float2 gridVal = abs(fract(coord - 0.5) - 0.5) / derivative;
    float lineVal = min(gridVal.x, gridVal.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    float4 color;
    if (game2D != 1)
        color = float4(0.2, 0.2, 0.2, 0.8 - min(lineVal, 1.0));
    else
        color = float4(0.2, 0.2, 0.2, 0.8 - min(lineVal, 1.0));

    if (game2D != 1)
    {
        if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
            color.z = 1.0;
        if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
            color.x = 1.0;
    }
    else
    {
        if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
            color.y = 1.0;
        if (fragPos3D.y > -0.1 * minimumz && fragPos3D.y < 0.1 * minimumz)
            color.x = 1.0;
    }

    return float4(color.xyz, color.w);
}

float calculateAttenuation(float range, float3 cameraPosition, float3 worldPoint)
{
    float constant;
    float linear;
    float quadratic;

    int rangeInt = (int)range;
    if (rangeInt == 7)      { constant = 1.0; linear = 0.7;    quadratic = 1.8;   }
    else if (rangeInt == 13) { constant = 1.0; linear = 0.35;   quadratic = 0.44;  }
    else if (rangeInt == 20) { constant = 1.0; linear = 0.22;   quadratic = 0.20;  }
    else if (rangeInt == 50) { constant = 1.0; linear = 0.14;   quadratic = 0.07;  }
    else if (rangeInt == 65) { constant = 1.0; linear = 0.09;   quadratic = 0.032; }
    else if (rangeInt == 100){ constant = 1.0; linear = 0.07;   quadratic = 0.017; }
    else if (rangeInt == 160){ constant = 1.0; linear = 0.045;  quadratic = 0.0075;}
    else if (rangeInt == 200){ constant = 1.0; linear = 0.027;  quadratic = 0.0028;}
    else if (rangeInt == 325){ constant = 1.0; linear = 0.022;  quadratic = 0.0019;}
    else if (rangeInt == 600){ constant = 1.0; linear = 0.014;  quadratic = 0.0007;}
    else if (rangeInt == 3250){constant = 1.0; linear = 0.007;  quadratic = 0.0002;}
    else                     { constant = 1.0; linear = 1.0;    quadratic = 1.0;   }

    float dist = length(cameraPosition.xz - worldPoint.xz);
    float att = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

    float fadeLimit    = 25;
    float outFadeLimit = 28;
    float epsilon = fadeLimit - outFadeLimit;
    float intensity = clamp((dist - outFadeLimit) / epsilon, 0.0, 1.0);

    return att * intensity;
}

float4 main(Input input) : SV_Target0
{
    float4 result = float4(0.0, 0.0, 0.0, 0.0);
    float attenuation = calculateAttenuation(65, input.CameraPosition, input.WorldPoint.xyz);

    int patterns = (int)input.PatternNumber;
    for (int i = 0; i < patterns; i++)
    {
        if (input.Game2D != 1)
            result += grid(input.WorldPoint.xyz, input.PatternSizes[i], true, input.Game2D) * attenuation;
        else
            result += grid(input.WorldPoint.xyz, input.PatternSizes[i], true, input.Game2D);
    }

    return float4(result.xyz, result.w * input.Opacity);
}
