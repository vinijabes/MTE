#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec3 a_Offset;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;
uniform vec3 u_Eye;

out vec2 v_TexCoord;
out vec4 v_Color;
flat out vec3 v_Normal;
out vec3 v_Position;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = vec4(1.0);
	v_Normal = a_Normal;
	v_Position = vec3(u_Transform * vec4(a_Position  + a_Offset, 1.f));

	gl_Position = u_ViewProjection * vec4(v_Position, 1.f);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
flat in vec3 v_Normal;
in vec3 v_Position;

struct Light 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light u_Lights[12];
uniform int   u_LightCount;
uniform vec3  u_ViewerPos;
uniform sampler2D u_Texture;

vec3 CalcLight(Light l, vec3 normal, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(u_ViewerPos - v_Position);
	vec3 result;

	for(int i = 0; i < u_LightCount; i++)
		result += CalcLight(u_Lights[i], v_Normal, viewDir);

	color = v_Color * vec4(result * 2, 1.f);
}

vec3 CalcLight(Light l, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-l.direction);

	float diff = max(dot(normal, lightDir), 0.f);

	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), 10);

	vec3 diffuse = l.diffuse * diff;
	vec3 specular = l.specular * spec;

	return diffuse + specular;
}
