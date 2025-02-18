#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;	

void main()
{
	color = vec4(0.8, 0.8, 0.8, 1.0);//texture(u_Texture, v_TexCoord * 10.0) * u_Color;
}

