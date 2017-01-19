#version 330 core

in vec2 exTexcoord;

layout(location = 0) out vec4 FragmentColor;

uniform vec3 Color;
uniform sampler2D Texture;

void main(void){

	//FragmentColor = Color;
	
  FragmentColor = texture(Texture, exTexcoord);
	if(FragmentColor.a < 0.5)
    discard;
}