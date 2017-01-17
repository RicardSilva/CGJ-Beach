#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform vec3 Color;
uniform sampler2D Texture;
uniform sampler2D permTexture;

//varying vec3 v_texCoord3D;
//varying vec4 v_color;

#define ONE 0.00390625
#define ONEHALF 0.001953125

uniform mat4 ModelMatrix;
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

float fade(float t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float noise(vec2 P)
{
  vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
  vec2 Pf = fract(P);             // Fractional part for interpolation

  // Noise contribution from lower left corner
  vec2 grad00 = texture(permTexture, Pi).rg * 4.0 - 1.0;
  float n00 = dot(grad00, Pf);

  // Noise contribution from lower right corner
  vec2 grad10 = texture(permTexture, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
  float n10 = dot(grad10, Pf - vec2(1.0, 0.0));

  // Noise contribution from upper left corner
  vec2 grad01 = texture(permTexture, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
  float n01 = dot(grad01, Pf - vec2(0.0, 1.0));

  // Noise contribution from upper right corner
  vec2 grad11 = texture(permTexture, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
  float n11 = dot(grad11, Pf - vec2(1.0, 1.0));

  // Blend contributions along x
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(Pf.x));

  // Blend contributions along y
  float n_xy = mix(n_x.x, n_x.y, fade(Pf.y));

  return n_xy;
}

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main(void)
{
	vec3 cameraPosition = -ViewMatrix[3].xyz * mat3(ViewMatrix);
	float color = noise(2*exTexcoord);
	vec4 c = vec4(color*0.3,color*0.3,color*0.3,1);
	
	//// lighning

	
    vec3 normal = normalize(transpose(inverse(mat3(ModelMatrix))) * exNormal);
    vec3 surfacePos = vec3(ModelMatrix * vec4(exPosition, 1));
    vec4 surfaceColor = c;//texture(Texture, exTexcoord);//
    vec3 surfaceToLight = normalize(lightPosition - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
    
    //ambient
    vec3 ambient = ambientCoefficient * surfaceColor.rgb * intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(exNormal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * 0.2 * surfaceColor.rgb  * intensities;

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, exNormal))), 0.9);
    vec3 specular = specularCoefficient * 0.7 * surfaceColor.rgb  * intensities;

    //attenuation
    float distanceToLight = length(lightPosition - exPosition);
    float lattenuation = 1.0 / (1.0 + attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + lattenuation*(diffuse + specular);
    
    // gamma for correction
    vec3 gamma = vec3(1.0/2.2);
	
	//FragmentColor = c;
	FragmentColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}