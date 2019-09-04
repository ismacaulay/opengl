#version 330 core
in vec3 v_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

out vec4 FragColor;

void main() {
    // FragColor = vec4(v_color, 1.0);

    vec2 flippedCoord = vec2(-v_tex_coord.s, v_tex_coord.t);
    FragColor = mix(texture(u_texture1, v_tex_coord), texture(u_texture2, flippedCoord), 0.2) *
                vec4(v_color, 1.0);
};
