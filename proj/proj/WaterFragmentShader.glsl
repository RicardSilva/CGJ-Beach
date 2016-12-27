#version 330 core


uniform vec3 Color;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;
uniform sampler2D dudvMap;

uniform float movementFactor;


in vec4 clipSpaceCoords;
in vec2 exTexcoord;

layout(location = 0) out vec3 FragmentColor;

const float distortionStrength = 0.01;

void main(void){

	vec2 ndc = (clipSpaceCoords.xy/clipSpaceCoords.w) / 2 + 0.5; // normalized device coords
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	
	vec2 distortion1 = (texture(dudvMap, vec2(exTexcoord.x + movementFactor, exTexcoord.y)).rg * 2.0 - 1.0) * distortionStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(-exTexcoord.x + movementFactor, exTexcoord.y + movementFactor)).rg * 2.0 - 1.0) * distortionStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
	
	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	
	vec4 reflectColor = texture(ReflectionTexture, reflectTexCoords);
  vec4 refractColor = texture(RefractionTexture, refractTexCoords);

	FragmentColor = mix(reflectColor, refractColor, 0.5).xyz;
	FragmentColor = mix(FragmentColor, vec3(0.0, 0.3, 0.5), 0.2);
	
	
	
	//FragmentColor = texture(Texture, exTexcoord).xyz;
}

