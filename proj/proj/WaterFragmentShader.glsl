#version 330 core


uniform vec3 Color;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float movementFactor;


in vec4 clipSpaceCoords;
in vec2 exTexcoord;
in vec3 toCameraVector;
in vec3 fromLightVector;
in vec3 exintensities; //a.k.a the color of the light
in float exattenuation; 
in float exambientCoefficient; 

layout(location = 0) out vec3 FragmentColor;

const float distortionStrength = 0.01;

void main(void){

	vec2 ndc = (clipSpaceCoords.xy/clipSpaceCoords.w) / 2 + 0.5; // normalized device coords
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	
	vec2 distortedTexCoords = texture(dudvMap, vec2(exTexcoord.x + movementFactor, exTexcoord.y)).rg*0.1;
	distortedTexCoords = exTexcoord + vec2(distortedTexCoords.x, distortedTexCoords.y+movementFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * distortionStrength;
	

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
	
	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	
	vec4 reflectColor = texture(ReflectionTexture, reflectTexCoords);
  vec4 refractColor = texture(RefractionTexture, refractTexCoords);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
	refractiveFactor = pow(refractiveFactor, 2);
	
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);
	
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, 20.0);
	vec3 specularHighlights = exintensities * specular * 0.4;
	
	FragmentColor = mix(reflectColor, refractColor, refractiveFactor).xyz;
	FragmentColor = mix(FragmentColor, vec3(0.0, 0.3, 0.5), 0.2) + specularHighlights;
	
	
}

