#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

out vec3 v_color;
out vec2 v_tex_coord;

void main() {
    v_color = a_color;
    v_tex_coord = a_tex_coord;

    gl_Position = vec4(a_pos, 1.0);
};
