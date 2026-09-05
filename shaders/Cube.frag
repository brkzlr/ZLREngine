#version 330 core
out vec4 o_fragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutoffAngle;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform Material u_material;
uniform vec3 u_viewPos;
uniform bool u_isFlashlightOn;

uniform DirLight u_sun;
uniform PointLight u_pointLight;
uniform SpotLight u_flashlight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
	vec3 norm = normalize(v_normal);
	vec3 viewDir = normalize(u_viewPos - v_fragPos);

	//Directional Lighting
	vec3 result = CalcDirLight(u_sun, norm, viewDir);

	//Point Lights
	result += CalcPointLight(u_pointLight, norm, v_fragPos, viewDir);

	//Spotlight
	if (u_isFlashlightOn == true){
		result += CalcSpotLight(u_flashlight, norm, v_fragPos, viewDir);
	}

	o_fragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize (-light.direction);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	
	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	//Result
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);

	//Diffuse
	float diff = max(dot(normal,lightDir), 0.0);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	//Attenuation
	float dist = length(light.position - fragPos);
	float atten = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//Result
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));

	ambient *= atten;
	diffuse *= atten;
	specular *= atten;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);

	//Diffuse
	float diff = max(dot(normal,lightDir), 0.0);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	//Attenuation
	float dist = length(light.position - fragPos);
	float atten = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//Spotlight Cone
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutoffAngle - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	//Result
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texCoord));
	vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));

	ambient *= atten * intensity;
	diffuse *= atten * intensity;
	specular *= atten * intensity;

	return (ambient + diffuse + specular);
}
