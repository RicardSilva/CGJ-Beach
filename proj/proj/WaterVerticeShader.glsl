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

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0);
	exTexcoord = vec2(inTexcoord.x, inTexcoord.y);
}