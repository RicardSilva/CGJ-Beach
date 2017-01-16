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



void main()
{
		mat4 inverseProjection = inverse(ProjectionMatrix);
		mat3 inverseModelview = transpose(mat3(ViewMatrix * ModelMatrix ));
		vec3 unprojected = (inverseProjection * vec4(inPosition,1)).xyz;
		TexCoords = inverseModelview * unprojected;

		gl_Position = vec4(inPosition,1);

}  