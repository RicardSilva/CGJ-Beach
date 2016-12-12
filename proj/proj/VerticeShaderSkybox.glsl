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
    vec4 pos = ProjectionMatrix* ViewMatrix* ModelMatrix *vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
    TexCoords = inPosition;
    //TexCoords = inTexcoord;
}  