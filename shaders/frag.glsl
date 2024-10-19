#version 410 core

in vec3 v_vertexColors;
in vec2 texCoord;

out vec4 color;

uniform mat4 u_ModelMatrix;
uniform sampler2D u_Tex0;
uniform vec4 u_lightColor;

void main()
{
   // color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);

   color = texture(u_Tex0 ,texCoord)*u_lightColor;
}