#version 330 core

in vec3 inPosition;

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

void main(void){

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0);
}