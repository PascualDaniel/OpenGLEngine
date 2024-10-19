#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 aTex;
layout(location = 3) in vec3 aNormal;


out vec2 texCoord;

out vec3 v_vertexColors;

out vec3 v_normal;
out vec3 v_position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;
//uniform sampler2D u_Tex0;


void main()
{
	v_position = vec3(u_ModelMatrix * vec4(position,1.0f));
	v_vertexColors= vertexColors;

	vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(v_position,1.0f);
    gl_Position = vec4(newPosition.x,newPosition.y , newPosition.z,newPosition.w);

	texCoord = aTex;
	v_normal = aNormal;
}