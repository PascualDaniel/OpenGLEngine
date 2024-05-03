#version 410 core

in vec3 v_vertexColors;

out vec4 color;



uniform mat4 u_ModelMatrix;

//Texturas
in vec2 texCoord;
uniform sampler2D u_Tex0;

void main()
{
   // color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);

   color = texture(u_Tex0 ,texCoord);
}