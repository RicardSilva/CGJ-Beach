#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;
out vec4 clipSpaceCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;
out vec3 exintensities; //a.k.a the color of the light
out float exattenuation; 
out float exambientCoefficient; 

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

const float tilling = 1.0; //FIXME: ADJUST TO WATER QUAD SIZE

void main(void){

	vec4 worldCoords =  ModelMatrix * vec4(inPosition,1.0);
	clipSpaceCoords = ProjectionMatrix * ViewMatrix * worldCoords;
	gl_Position = clipSpaceCoords;
	exTexcoord = vec2(inTexcoord.x, -inTexcoord.y) * tilling;
	
	vec3 cameraPosition = -ViewMatrix[3].xyz * mat3(ViewMatrix);
	toCameraVector = cameraPosition - worldCoords.xyz;
	fromLightVector = worldCoords.xyz - vec3(0,5,0);
	
  exintensities = intensities; 
	exattenuation = attenuation; 
	exambientCoefficient = ambientCoefficient; 
}