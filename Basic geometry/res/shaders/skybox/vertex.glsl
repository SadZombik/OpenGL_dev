#version 330 core
layout (location = 0) in vec3 pos;

out vec3 frag_tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    frag_tex_coords = pos;
    vec4 position = projection * view * vec4(pos, 1.0);
    gl_Position = position.xyww;
}  