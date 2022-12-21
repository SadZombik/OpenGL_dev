// surface
#version 330 core
struct Material {
    sampler2D diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex_coords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ќкружающа€ составл€юща€
    vec3 ambient = light.ambient * texture(material.diffuse, frag_tex_coords).rgb;
  	
    // ƒиффузна€ составл€юща€ 
    vec3 norm = normalize(frag_normal);
    vec3 lightDir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, frag_tex_coords).rgb;  
    
    // ќтраженна€ составл€юща€
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0);
} 
