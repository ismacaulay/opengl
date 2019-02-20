#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position;
// layout(location = 1) in vec3 a_color;
layout(location = 1) in vec2 a_texCoord;

out vec3 v_color;
out vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
   gl_Position = u_proj * u_view * u_model * a_position;
   // v_color = a_color;
   v_texCoord = a_texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 frag_color;

in vec3 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_mix;

void main() {
   // frag_color = texture(u_texture, v_texCoord) * vec4(v_color, 1.0);
   frag_color = mix(texture(u_texture1, vec2(1.0f - v_texCoord.x, v_texCoord.y)), texture(u_texture2, v_texCoord), u_mix);
}
