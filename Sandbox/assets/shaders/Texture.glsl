#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_TexCoords;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = vec2(u_TexCoords.x + a_TexCoord.x * (u_TexCoords.z - u_TexCoords.x), u_TexCoords.y + a_TexCoord.y * (u_TexCoords.w - u_TexCoords.y));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
			
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord) * u_Color;
}

