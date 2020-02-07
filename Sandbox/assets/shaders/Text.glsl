#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_TexCoords;

void main()
{
    TexCoords = vec2(u_TexCoords.x + a_TexCoord.x * (u_TexCoords.z - u_TexCoords.x), u_TexCoords.y + a_TexCoord.y * (u_TexCoords.w - u_TexCoords.y));;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}  

#type fragment
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, TexCoords).r);
    color = vec4(u_TextColor, 1.0) * sampled;
}  