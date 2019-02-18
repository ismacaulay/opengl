#include <iostream>

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"

#include <GLFW/glfw3.h> // must be included after renderer.h

float mixParam = 0.5f;

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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    unsigned int width = 800;
    unsigned int height = 600;
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

    float positions[] = {
        // positions       // colors         // tex coords
        -0.5f, -0.5f, 0.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0: x, y, s, t
         0.5f, -0.5f, 0.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 0.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    VertexBuffer vb(positions, 4 * 8 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
    layout.push<float>(2);
    IndexBuffer ib(indices, 6);

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
    shader.setUniform1i("u_texture1", 1);
    texture2.bind(1);
    shader.setUniform1i("u_texture2", 0);

    // va.unbind();
    // shader.unbind();
    // vb.unbind();
    // ib.unbind();

    Renderer renderer;

    // float r = 0.0;
    // float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderer.clear();

        shader.bind();
        // shader.setUniform4f("u_color", r, 0.4f, 0.8f, 1.0f);
        // shader.setUniform1f("u_offset", r);
        shader.setUniform1f("u_mix", mixParam);

        renderer.draw(va, ib, shader);

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
