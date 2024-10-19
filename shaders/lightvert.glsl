#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 aTex;
layout(location = 3) in vec3 aNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;
//uniform sampler2D u_Tex0;

out vec2 texCoord;

out vec3 v_vertexColors;

void main()
{
	v_vertexColors= vertexColors;

	vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);

    gl_Position = vec4(newPosition.x,newPosition.y , newPosition.z,newPosition.w);

	texCoord = aTex;
}