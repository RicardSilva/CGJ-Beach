#version 330 core
in vec3 TexCoords;
out vec4 color;

smooth in vec3 eyeDirection;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
}