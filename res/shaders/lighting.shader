#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 v_frag_position;
out vec3 v_normal;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
    v_frag_position = vec3(u_model * vec4(a_position, 1.0));
    v_normal = mat3(transpose(inverse(u_model))) * a_normal; // inverse calcs are expensive, do it cpu side
}

#shader fragment
#version 330 core

in vec3 v_frag_position;
in vec3 v_normal;

layout(location = 0) out vec4 frag_color;

uniform vec3 u_light_position;
uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 u_camera_position;

void main() {
    float ambient_str = 0.1;
    vec3 ambient = ambient_str * u_light_color;

    vec3 normal = normalize(v_normal);
    vec3 light_dir = normalize(u_light_position - v_frag_position);

    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;

    float specular_str = 0.5;
    vec3 view_dir = normalize(u_camera_position - v_frag_position);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_str * spec * u_light_color;

    vec3 result = (ambient + diffuse + specular) * u_object_color;
    frag_color = vec4(result, 1.0);
}
