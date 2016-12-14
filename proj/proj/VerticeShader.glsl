#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 lightPosition;
    vec3 intensities; //a.k.a the color of the light
    float attenuation; 
    float ambientCoefficient; 
};


void main(void)
{
	exPosition = inPosition;
	exTexcoord = vec2(inTexcoord.x, 1.0-inTexcoord.y);
	exNormal = inNormal;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}