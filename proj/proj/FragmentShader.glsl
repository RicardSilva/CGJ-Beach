#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform vec3 Color;
uniform sampler2D Texture;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 position;
    vec3 intensities; //a.k.a the color of the light
    float attenuation; 
    float ambientCoefficient; 
};


void main(void)
{
	vec3 cameraPosition = vec3(0,1,-1);//-ViewMatrix[3].xyz * mat3(ViewMatrix);
    vec3 normal = normalize(transpose(inverse(mat3(ModelMatrix))) * exNormal);
    vec3 surfacePos = vec3(ModelMatrix * vec4(exPosition, 1));
    vec4 surfaceColor = texture(Texture, exTexcoord);
    vec3 surfaceToLight = normalize(position - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
    
    //ambient
    vec3 ambient = ambientCoefficient * surfaceColor.rgb * intensities;
	//FragmentColor = vec4(ambient, surfaceColor.a);

    //diffuse
    float diffuseCoefficient = max(0.0, dot(exNormal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * intensities;
   // FragmentColor = vec4(diffuse, 1.0);

    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, exNormal))), 2);
    vec3 specular = specularCoefficient * surfaceColor.rgb * intensities;
    //attenuation
    float distanceToLight = length(position - exPosition);
    float lattenuation = 1.0 / (1.0 + attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + lattenuation*(diffuse + specular);
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    FragmentColor = vec4(pow(linearColor, gamma), surfaceColor.a);
	  //FragmentColor = vec4(linearColor, 1.0);
    		//FragmentColor = texture(Texture, exTexcoord);	
}