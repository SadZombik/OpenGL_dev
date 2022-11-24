#version 330 core
in vec2 frag_tex_coords;
uniform sampler2D texture1;

void main()
{    
    gl_FragColor = texture(texture1, frag_tex_coords);
}