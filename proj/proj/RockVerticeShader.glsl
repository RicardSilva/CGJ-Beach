#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform float time;
uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec4 ClipingPlane;
	vec3 lightPosition;
	vec3 intensities; //a.k.a the color of the light
	float attenuation; 
	float ambientCoefficient; 
};

void main(void)
{

	vec4 WorldPosition = ModelMatrix *vec4(inPosition.x,inPosition.y,inPosition.z, 1.0);
	gl_ClipDistance[0] = dot(WorldPosition, ClipingPlane);
	
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = inNormal;

	
	
	//vec4 MCPosition = vec4(inPosition.x,inPosition.y,inPosition.z, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;
}