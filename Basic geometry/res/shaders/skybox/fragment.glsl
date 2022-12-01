#version 330 core
out vec4 FragColor;

in vec3 frag_tex_coords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, frag_tex_coords);
}
