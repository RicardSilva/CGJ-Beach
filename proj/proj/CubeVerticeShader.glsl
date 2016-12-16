#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
  vec4 ClipingPlane;
};

void main(void){

	vec4 WorldPosition = ModelMatrix * vec4(inPosition,1.0);
	gl_ClipDistance[0] = dot(WorldPosition, ClipingPlane);
	
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;
	exTexcoord = vec2(inTexcoord.x, 1.0-inTexcoord.y);
}