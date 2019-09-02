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

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_frag_position;
in vec3 v_normal;

uniform vec3 u_camera_position;
uniform Material material;
uniform Light light;

layout(location = 0) out vec4 frag_color;

void main() {
    // ambient
    vec3 ambient =  light.ambient * material.ambient;

    // diffuse
    vec3 normal = normalize(v_normal);
    vec3 light_dir = normalize(light.position - v_frag_position);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 view_dir = normalize(u_camera_position - v_frag_position);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}
