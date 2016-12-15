#version 330 core

out vec3 TexCoords;

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

smooth out vec3 eyeDirection;

void main()
{
		mat4 modifiedViewMatrix = ViewMatrix;
		modifiedViewMatrix[0][2] = 0;
		modifiedViewMatrix[1][2] = 0;
		modifiedViewMatrix[2][2] = 0;
    vec4 pos = ProjectionMatrix * modifiedViewMatrix * ModelMatrix *vec4(inPosition, 1.0);
    gl_Position = pos;
    TexCoords = inPosition;
}  