#version 330 core


out vec4 FragmentColor;

uniform vec3 Color;

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
	
	FragmentColor = vec4(Color, 1.0);
	
}