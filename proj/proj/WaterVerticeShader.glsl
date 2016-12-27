#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;
out vec4 clipSpaceCoords;
out vec3 toCameraVector;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec4 ClipingPlane;
};

const float tilling = 6.0;

void main(void){

	vec4 worldCoords =  ModelMatrix * vec4(inPosition,1.0);
	clipSpaceCoords = ProjectionMatrix * ViewMatrix * worldCoords;
	gl_Position = clipSpaceCoords;
	exTexcoord = vec2(inTexcoord.x, -inTexcoord.y) * tilling;
	
	//mat4 invModelViewMatrix = inverse(ViewMatrix * ModelMatrix);
	//vec3 cameraPosition = (vec3(invModelViewMatrix[3])) / (invModelViewMatrix[3][3]);
	
	vec3 cameraPosition = -ViewMatrix[3].xyz * mat3(ViewMatrix);
	
	toCameraVector = cameraPosition - worldCoords.xyz;
}