#version 330 core
out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//uniform vec3 lightPos;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
// map
struct Material1{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
//specular  map
struct Material2{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light
{
    vec3 position;
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material2 material;

void main()
{

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);//(light.position - FragPos);

	//diffuse
	float diff = max(dot(norm,lightDir),0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords) ) ;//*lightColor;
	
	//specular
	//float specularStrength = 0.8f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max( dot(viewDir, reflectDir), 0. ),material.shininess);
	vec3 specular=  light.specular *(vec3(1.0f)- vec3(texture(material.specular, TexCoords) )) * spec ;//*lightColor;


	//ambient
	//float ambientStrength = 0.1f;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords) ); // *lightColor;


	vec3 result =(specular+ diffuse+ ambient) ;//* objectColor;

    color = vec4(result, 1.0f);
}