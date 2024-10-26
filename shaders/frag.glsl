#version 410 core

in vec3 v_vertexColors;
in vec2 texCoord;

in vec3 v_normal;
in vec3 v_position;

out vec4 color;

uniform mat4 u_ModelMatrix;
uniform sampler2D u_Tex0;
uniform sampler2D u_Tex1;
uniform vec4 u_lightColor;
uniform vec3 u_lightPos;

uniform vec3 u_viewPos;

vec4 pointLight(){
    
    vec3 lightVector = u_lightPos- v_position;
    float distance = length(lightVector);
    float a = 0.3;
    float b = 0.7;
    float intensity = 1.0 / (a  * distance * distance + b * distance+1.0);

   float ambientStrength = 0.2f;

   vec3 normal = normalize(v_normal);
   vec3 lightDir = normalize(lightVector);
   float diff = max(dot(normal, lightDir), 0.0f);


    
   float specularStrength = 0.5f;
   vec3 viewDir = normalize(u_viewPos-v_position);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularStrength * spec ;
   return (texture(u_Tex0 ,texCoord) * (diff*intensity+ambientStrength) + texture(u_Tex1 ,texCoord).r * specular*intensity ) * u_lightColor;

}

vec4 directionalLight(){
   float ambientStrength = 0.2f;

   vec3 normal = normalize(v_normal);
   vec3 lightDir = normalize(vec3(1.0f,1.0f,0.0f));
   float diff = max(dot(normal, lightDir), 0.0f);


    
   float specularStrength = 0.5f;
   vec3 viewDir = normalize(u_viewPos-v_position);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularStrength * spec ;
   return (texture(u_Tex0 ,texCoord) * (diff+ambientStrength) + texture(u_Tex1 ,texCoord).r * specular ) * u_lightColor;

}
vec4 spotLight(){
   float outerCone = 0.90f;
   float innerCone = 0.95f;


   float ambientStrength = 0.2f;

   vec3 normal = normalize(v_normal);
   vec3 lightDir = normalize(u_lightPos- v_position);
   float diff = max(dot(normal, lightDir), 0.0f);


    
   float specularStrength = 0.5f;
   vec3 viewDir = normalize(u_viewPos-v_position);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
   float specular = specularStrength * spec ;

   float angle = dot(vec3(-1.0f,0.0f,-1.0f), -lightDir);
   float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

   return (texture(u_Tex0 ,texCoord) * (diff*inten+ambientStrength) + texture(u_Tex1 ,texCoord).r * specular*inten ) * u_lightColor;

}

void main()
{
    color = spotLight();
}