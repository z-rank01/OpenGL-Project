#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

void main()
{    
    vec3 worldnormal = normalize(normal);
    vec3 worldpos = normalize(pos);
    vec3 lightpos = vec3(1.0, 1.0, 1.0);

    vec3 lightdir = normalize(lightpos - worldpos);
    vec3 reflectdir = reflect(-lightdir, worldnormal);
    vec3 viewdir = normalize(viewPos - worldpos);

    float diffusefactor = max(dot(worldnormal, lightdir), 0.0);
    float specularfactor = pow(max(dot(viewdir, reflectdir), 0.0), 32.0);

    vec3 ambient = texture(texture_diffuse1, TexCoords).rgb * vec3(0.5, 0.5, 0.5);
    vec3 diffuse = texture(texture_diffuse1, TexCoords).rgb * vec3(1.0, 1.0, 1.0) * diffusefactor;
    vec3 specular = texture(texture_specular1, TexCoords).rgb * vec3(0.5, 0.5, 0.5) * specularfactor;


    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = texture(texture_diffuse1, TexCoords);
}