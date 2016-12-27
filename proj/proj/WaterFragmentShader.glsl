#version 330 core


uniform vec3 Color;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;

//in vec2 exTexcoord;
in vec4 clipSpaceCoords;

layout(location = 0) out vec3 FragmentColor;

void main(void){

	vec2 ndc = (clipSpaceCoords.xy/clipSpaceCoords.w) / 2 + 0.5; // normalized device coords
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	


	vec4 reflectColor = texture(ReflectionTexture, reflectTexCoords);
  vec4 refractColor = texture(RefractionTexture, refractTexCoords);

	FragmentColor = mix(reflectColor, refractColor, 0.5).xyz;
	
	
	
	//FragmentColor = texture(Texture, exTexcoord).xyz;
}

