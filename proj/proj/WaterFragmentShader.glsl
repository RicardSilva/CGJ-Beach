#version 330 core

out vec4 FragmentColor;
uniform vec3 Color;

void main(void){

	FragmentColor = vec4(Color,1.0);

}