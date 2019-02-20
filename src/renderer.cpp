#include "renderer.h"

#include <iostream>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

void glClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "[opengl error] " << file << "::" << function << ":" << line << ": " << err << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {

}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    va.bind();
    shader.bind();
    ib.bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
