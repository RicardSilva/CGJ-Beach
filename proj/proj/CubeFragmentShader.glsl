#version 330 core

in vec2 exTexcoord;

layout(location = 0) out vec3 FragmentColor;

uniform vec3 Color;
uniform sampler2D Texture;

void main(void){

	//FragmentColor = vec4(Color,1.0);
  FragmentColor = texture(Texture, exTexcoord).xyz;
}