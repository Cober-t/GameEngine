// Editor Infinite grid

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_CameraPosition;
layout(location = 2) in int  a_Game2D;
layout(location = 3) in vec4 a_PatternSizes;
layout(location = 4) in float a_PatternNumber;
layout(location = 5) in float a_Opacity;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_Projection;
	mat4 u_View;
};

layout(location = 0) out vec3 cameraPosition;
layout(location = 1) out vec4 worldPoint;
layout(location = 2) out vec4 worldCamera;
layout(location = 3) out flat int game2D;
layout(location = 4) out flat vec4 patternSizes;
layout(location = 5) out flat float patternNumber;
layout(location = 6) out flat float opacity;

void main() 
{
    vec4 point = u_Projection * u_View * vec4(a_Position, 1.0);
    gl_Position = point;

    cameraPosition = a_CameraPosition;
    worldPoint = vec4(a_Position, 1.0);
    worldCamera = point;
    game2D = a_Game2D;
	
	patternSizes = a_PatternSizes;	
	patternNumber = a_PatternNumber;
    opacity = a_Opacity;
}

#type fragment
#version 460 core

layout(location = 0) in vec3 cameraPosition;
layout(location = 1) in vec4 worldPoint;
layout(location = 2) in vec4 worldCamera;
layout(location = 3) in flat int game2D;
layout(location = 4) in flat vec4 patternSizes;
layout(location = 5) in flat float patternNumber;
layout(location = 6) in flat float opacity;

layout(location = 0) out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) 
{
    vec2 coord;
    if (game2D != 1)
        coord = fragPos3D.xz * scale;
    else
        coord = fragPos3D.xy * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    vec4 color;
    if (game2D != 1)
        color = vec4(0.2, 0.2, 0.2, 0.8 - min(line, 1.0));
    else
        color = vec4(0.2, 0.2, 0.2, 0.8 - min(line, 1.0));
    float alpha = color.w;
    
    if (game2D != 1)
    {
        if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
            color.z = 1.0;
        if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
            color.x = 1.0;
    }
    else 
    {
        if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
            color.y = 1.0;
        if(fragPos3D.y > -0.1 * minimumz && fragPos3D.y < 0.1 * minimumz)
            color.x = 1.0;
    }
    
    return vec4(color.xyz, alpha);
}

float calculateAttenuation(float range) 
{
    float constant;
    float linear;
    float quadratic;

    switch(int(range)) 
    {
        case 7:     constant = 1.0;  linear = 0.7;    quadratic = 1.8;      break;
        case 13:    constant = 1.0;  linear = 0.35;   quadratic = 0.44;     break;
        case 20:    constant = 1.0;  linear = 0.22;   quadratic = 0.20;     break;
        case 50:    constant = 1.0;  linear = 0.14;   quadratic = 0.07;     break;
        case 65:    constant = 1.0;  linear = 0.09;   quadratic = 0.032;    break;
        case 100:   constant = 1.0;  linear = 0.07;   quadratic = 0.017;    break;
        case 160:   constant = 1.0;  linear = 0.045;  quadratic = 0.0075;   break;
        case 200:   constant = 1.0;  linear = 0.027;  quadratic = 0.0028;   break;
        case 325:   constant = 1.0;  linear = 0.022;  quadratic = 0.0019;   break;
        case 600:   constant = 1.0;  linear = 0.014;  quadratic = 0.0007;   break;
        case 3250:  constant = 1.0;  linear = 0.007;  quadratic = 0.0002;   break;
        default:    constant = 1.0;  linear = 1.0;    quadratic = 1.0;      break;
    }

    vec3 origin = cameraPosition;
    float distance = length(origin.xz - worldPoint.xz);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    float fadeLimit    = 25;
    float outFadeLimit = 28;

    float epsilon = fadeLimit - outFadeLimit; // CutOff and OuterCutOff angles
    float intensity = clamp((distance - outFadeLimit) / epsilon, 0.0, 1.0);

    return attenuation * intensity;
}

void main() 
{
    vec4 result = vec4(0.0f);
    float attenuation = calculateAttenuation(65);
	
	for (int i = 0; i < int(patternNumber); i++) 
    {
		if (game2D != 1)
        	result += grid(worldPoint.xyz, patternSizes[i], true) * attenuation;
		else
        	result += grid(worldPoint.xyz, patternSizes[i], true);
	}

    outColor = vec4(result.xyz, result.w * opacity);
}