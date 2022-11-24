#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;

out vec2 frag_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    frag_tex_coords = tex_coords;    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}