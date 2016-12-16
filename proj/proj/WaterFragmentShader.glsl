#version 330 core


uniform vec3 Color;
uniform sampler2D Texture;

in vec2 exTexcoord;

layout(location = 0) out vec3 FragmentColor;

void main(void){

	//FragmentColor = vec4(Color,1.0);
	FragmentColor = texture(Texture, exTexcoord).xyz;
}

