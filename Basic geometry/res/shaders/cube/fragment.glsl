// cube
#version 330 core
in vec3 frag_pos;
in vec3 frag_normal;  
in vec2 frag_tex_coords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    // projector
    vec3 direction;
    float cutOff;
    float outerCutOff;

    // basic lighting
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Окружающая составляющая
    vec3 ambient = light.ambient * texture(material.diffuse, frag_tex_coords).rgb;
    
    // Диффузная составляющая 
    vec3 norm = normalize(frag_normal);
    vec3 lightDir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, frag_tex_coords).rgb;  
    
    // Отраженная составляющая
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, frag_tex_coords).rgb;  
    
    // Прожектор
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // Затухание
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0);
} 