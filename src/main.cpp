#include <iostream>

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h> // must be included after renderer.h

float mixParam = 0.5f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixParam += 0.01f;
        if (mixParam > 1.0f) {
            mixParam = 1.0f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixParam -= 0.01f;
        if (mixParam < 0.0f) {
            mixParam = 0.0f;
        }
    }

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

float fov = 45.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

int main(int argc, const char** argv) {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Failed to init glfw" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    float width = 800.0f;
    float height = 600.0f;
    GLFWwindow* window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* init glad - must be done before an opengl call */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // float triangle[] = {
    //      // positions  // colors
    //      0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   // bottom left
    //      0.0f,  0.5f,  0.0f, 0.0f, 1.0f    // top
    // };
    // unsigned int indices[] = {
    //     0, 1, 2,
    // };
    // VertexBuffer vb(triangle, 3 * 5 * sizeof(float));
    // VertexBufferLayout layout;
    // layout.push<float>(2);
    // layout.push<float>(3);
    // IndexBuffer ib(indices, 3);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    VertexBuffer vb(vertices, 36 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);

    // float positions[] = {
    //     // positions       // colors         // tex coords
    //     -0.5f, -0.5f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0: x, y, s, t
    //      0.5f, -0.5f, 0.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 1
    //      0.5f,  0.5f, 0.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
    //     -0.5f,  0.5f, 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 3
    // };
    // unsigned int indices[] = {
    //     0, 1, 2,
    //     2, 3, 0,
    // };
    // VertexBuffer vb(positions, 4 * 8 * sizeof(float));
    // VertexBufferLayout layout;
    // layout.push<float>(3);
    // layout.push<float>(3);
    // layout.push<float>(2);
    // IndexBuffer ib(indices, 6);

    VertexArray va;
    va.addBuffer(vb, layout);

    // Shader shader("res/shaders/basic.shader");
    Shader shader("res/shaders/texture.shader");
    // Shader shader("res/shaders/color.shader");
    shader.bind();
    // shader.setUniform4f("u_color", 0.2f, 0.4f, 0.8f, 1.0f);
    // shader.setUniform1f("u_offset", 0.25);
    shader.setUniform1f("u_mix", mixParam);

    Texture texture1("res/textures/awesomeface.png");
    Texture texture2("res/textures/ironmanicon.jpeg");

    texture1.bind(0);
    shader.setUniform1i("u_texture1", 0);
    texture2.bind(1);
    shader.setUniform1i("u_texture2", 1);

    // va.unbind();
    // shader.unbind();
    // vb.unbind();
    // ib.unbind();

    Renderer renderer;

    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    // glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

    // float r = 0.0;
    // float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        renderer.clear();

        // float radius = 20.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;

        // glm::mat4 view = glm::mat4(1.0f);
        // view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 view;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), cameraTarget, up);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 proj = glm::perspective(glm::radians(fov), width/height, 0.1f, 100.0f);

        shader.bind();
        shader.setUniformMat4f("u_view", view);
        shader.setUniformMat4f("u_proj", proj);

        // shader.setUniform4f("u_color", r, 0.4f, 0.8f, 1.0f);
        // shader.setUniform1f("u_offset", r);
        shader.setUniform1f("u_mix", mixParam);

        // renderer.draw(va, ib, shader);

        va.bind();
        for (int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float rotation = 20.0f * i;
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniformMat4f("u_model", model);

            GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        // if (r > 1.0f) {
        //     increment = -0.05f;
        // } else if (r < -1.0f) {
        //     increment = 0.05f;
        // }
        // r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
