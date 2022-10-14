#version 330 core

in vec3 worldnormal;
in vec3 fragpos;
in vec2 uv;

out vec4 outColor;

struct Material {
    sampler2D diffusemap;
    sampler2D specularmap;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlight[NR_POINT_LIGHTS];

vec3 ComputeDirLight(DirLight dirlight, vec3 normal, vec3 viewDir);
vec3 ComputePointLight(PointLight pointlight, vec3 normal, vec3 viewDir, vec3 fragpos);

void main()
{
    vec3 normal = normalize(worldnormal);
    vec3 viewDir = normalize(viewPos - fragpos);

    vec3 result = ComputeDirLight(dirlight, normal, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += ComputePointLight(pointlight[i], normal, viewDir, fragpos); 
    }

    outColor = vec4(result, 1.0);
    //outColor = vec4(1.0, 1.0, 1.0, 1.0);
}

vec3 ComputeDirLight(DirLight dirlight, vec3 normal, vec3 viewDir)
{
    vec3 lightdir = normalize(-dirlight.direction);

    //diffuse
    float diff = max(dot(normal, lightdir), 0.0);
    vec3 diffuseCol = diff * texture(material.diffusemap, uv).rgb * dirlight.diffuse;

    //ambient
    vec3 ambientCol =  texture(material.diffusemap, uv).rgb * dirlight.ambient;

    //specular
    vec3 reflectDir = reflect(-lightdir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularCol = texture(material.specularmap, uv).rgb * spec * dirlight.specular;

    return (ambientCol + diffuseCol + specularCol);
}

vec3 ComputePointLight(PointLight pointlight, vec3 normal, vec3 viewDir, vec3 fragpos)
{
    vec3 lightdir = normalize(pointlight.position - fragpos);

    //diffuse
    float diff = max(dot(normal, lightdir), 0.0);
    vec3 diffuseCol = diff * texture(material.diffusemap, uv).rgb * pointlight.diffuse;

    //ambient
    vec3 ambientCol =  texture(material.diffusemap, uv).rgb * pointlight.ambient;

    //specular
    vec3 reflectDir = reflect(-lightdir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularCol = texture(material.specularmap, uv).rgb * spec * pointlight.specular;

    //attenuation
    float distance = length(pointlight.position - fragpos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * distance * distance);

    return (ambientCol + diffuseCol + specularCol) * attenuation;
}
