#include <iostream>

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"
#include "defines.h"

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

    float width = 800.0f;
    float height = 600.0f;
    GLFWwindow* window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

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


    VertexBuffer vb(Objects::CubeWithNormals, 36 * 6 * sizeof(float));
    VertexBufferLayout cubeLayout;
    cubeLayout.push<float>(3);
    cubeLayout.push<float>(3);

    VertexArray cubeVA;
    cubeVA.addBuffer(vb, cubeLayout);

    VertexBufferLayout lampLayout;
    lampLayout.push<float>(3);
    lampLayout.push<float>(3);
    VertexArray lampVA;
    lampVA.addBuffer(vb, lampLayout);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    Shader lampShader("res/shaders/basic.shader");
    lampShader.bind();
    lampShader.setUniform4f("u_color", 1.0, 1.0, 1.0, 1.0);

    Shader cubeShader("res/shaders/lighting.shader");
    cubeShader.bind();
    cubeShader.setUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
    cubeShader.setUniform3f("material.diffuse", 1.0f, 0.5f, 0.31f);
    cubeShader.setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
    cubeShader.setUniform1f("material.shininess", 32.0f);

    cubeShader.setUniform3f("light.ambient",  0.2f, 0.2f, 0.2f);
    cubeShader.setUniform3f("light.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    cubeShader.setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);

    Renderer renderer;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        renderer.clear();

        glm::mat4 proj = glm::perspective(glm::radians(fov), width/height, 0.1f, 100.0f);
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        cubeVA.bind();
        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

        cubeShader.bind();
        cubeShader.setUniformMat4f("u_view", view);
        cubeShader.setUniformMat4f("u_proj", proj);
        cubeShader.setUniformMat4f("u_model", model);
        cubeShader.setUniform3f("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
        cubeShader.setUniform3f("light.diffuse", diffuseColor.z, diffuseColor.y, diffuseColor.z);

        cubeShader.setUniform3f("u_camera_position", cameraPos.x, cameraPos.y, cameraPos.z);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampVA.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lampShader.bind();
        lampShader.setUniformMat4f("u_view", view);
        lampShader.setUniformMat4f("u_proj", proj);
        lampShader.setUniformMat4f("u_model", model);

        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
