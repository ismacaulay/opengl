#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_color;

uniform float u_offset;

out vec4 v_color;
out vec4 v_pos;

void main() {
   gl_Position = vec4(-a_position.x + u_offset, -a_position.y, -a_position.z, 1.0);
   v_pos = vec4(-a_position.x + u_offset, -a_position.y, -a_position.z, 1.0);
   v_color = vec4(a_color, 1.0f);
}

#shader fragment
#version 330 core

in vec4 v_color;
in vec4 v_pos;

layout(location = 0) out vec4 color;

void main() {
   color = v_pos;
}
