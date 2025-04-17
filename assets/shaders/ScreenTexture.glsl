#type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 TexCoords;

void main()
{
    TexCoords = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
}  


#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;

layout (location = 0) in vec2 TexCoords;


uniform sampler2D u_ScreenTexture;

void main()
{
    vec3 col = texture(u_ScreenTexture, TexCoords).rgb;
    o_Color = vec4(col, 1.0);
}