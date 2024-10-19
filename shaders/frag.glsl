#version 410 core

in vec3 v_vertexColors;
in vec2 texCoord;

in vec3 v_normal;
in vec3 v_position;

out vec4 color;

uniform mat4 u_ModelMatrix;
uniform sampler2D u_Tex0;
uniform vec4 u_lightColor;
uniform vec3 u_lightPos;

uniform vec3 u_viewPos;

void main()
{

   float ambientStrength = 0.2f;

   vec3 normal = normalize(v_normal);
   vec3 lightDir = normalize(u_lightPos - v_position);
   float diff = max(dot(normal, lightDir), 0.0f);


    
   float specularStrength = 0.5f;
   vec3 viewDir = normalize(u_viewPos-v_position);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8);
   float specular = specularStrength * spec ;


    color = texture(u_Tex0 ,texCoord)*u_lightColor*(diff+ambientStrength+specular);
}